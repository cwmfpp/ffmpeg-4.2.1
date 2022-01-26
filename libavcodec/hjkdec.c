/*
 * HW decode acceleration through HJKDEC
 *
 * Copyright (c) 2016 Anton Khirnov
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "config.h"

#include "libavutil/common.h"
#include "libavutil/error.h"
#include "libavutil/hwcontext.h"
#include "libavutil/hwcontext_hjk_internal.h"
#include "libavutil/hjk_check.h"
#include "libavutil/pixdesc.h"
#include "libavutil/pixfmt.h"

#include "avcodec.h"
#include "decode.h"
#include "hjkdec.h"
#include "internal.h"

#if !HJKDECAPI_CHECK_VERSION(9, 0)
#define hjkVideoSurfaceFormat_YUV444 2
#define hjkVideoSurfaceFormat_YUV444_16Bit 3
#endif

typedef struct HJKDECDecoder {
    HJvideodecoder decoder;

    AVBufferRef *hw_device_ref;
    HJcontext    hjk_ctx;
    HJstream     stream;

    HjkFunctions *hjdl;
    HjvidFunctions *hvdl;
} HJKDECDecoder;

typedef struct HJKDECFramePool {
    unsigned int dpb_size;
    unsigned int nb_allocated;
} HJKDECFramePool;

#define CHECK_CU(x) FF_HJK_CHECK_DL(logctx, decoder->hjdl, x)

static int map_avcodec_id(enum AVCodecID id)
{
    switch (id) {
    case AV_CODEC_ID_H264:       return hjkVideoCodec_H264;
    case AV_CODEC_ID_HEVC:       return hjkVideoCodec_HEVC;
    case AV_CODEC_ID_MJPEG:      return hjkVideoCodec_JPEG;
    case AV_CODEC_ID_MPEG1VIDEO: return hjkVideoCodec_MPEG1;
    case AV_CODEC_ID_MPEG2VIDEO: return hjkVideoCodec_MPEG2;
    case AV_CODEC_ID_MPEG4:      return hjkVideoCodec_MPEG4;
    case AV_CODEC_ID_VC1:        return hjkVideoCodec_VC1;
    case AV_CODEC_ID_VP8:        return hjkVideoCodec_VP8;
    case AV_CODEC_ID_VP9:        return hjkVideoCodec_VP9;
    case AV_CODEC_ID_WMV3:       return hjkVideoCodec_VC1;
    }
    return -1;
}

static int map_chroma_format(enum AVPixelFormat pix_fmt)
{
    int shift_h = 0, shift_v = 0;

    av_pix_fmt_get_chroma_sub_sample(pix_fmt, &shift_h, &shift_v);

    if (shift_h == 1 && shift_v == 1)
        return hjkVideoChromaFormat_420;
    else if (shift_h == 1 && shift_v == 0)
        return hjkVideoChromaFormat_422;
    else if (shift_h == 0 && shift_v == 0)
        return hjkVideoChromaFormat_444;

    return -1;
}

static int hjkdec_test_capabilities(HJKDECDecoder *decoder,
                                   HJVIDDECODECREATEINFO *params, void *logctx)
{
    int ret;
    HJVIDDECODECAPS caps = { 0 };

    caps.eCodecType      = params->CodecType;
    caps.eChromaFormat   = params->ChromaFormat;
    caps.nBitDepthMinus8 = params->bitDepthMinus8;

    if (!decoder->hvdl->hjvidGetDecoderCaps) {
        av_log(logctx, AV_LOG_WARNING, "Used Nvidia driver is too old to perform a capability check.\n");
        av_log(logctx, AV_LOG_WARNING, "The minimum required version is "
#if defined(_WIN32) || defined(__CYGWIN__)
            "378.66"
#else
            "378.13"
#endif
            ". Continuing blind.\n");
        return 0;
    }

    ret = CHECK_CU(decoder->hvdl->hjvidGetDecoderCaps(&caps));
    if (ret < 0)
        return ret;

    av_log(logctx, AV_LOG_VERBOSE, "HJKDEC capabilities:\n");
    av_log(logctx, AV_LOG_VERBOSE, "format supported: %s, max_mb_count: %d\n",
           caps.bIsSupported ? "yes" : "no", caps.nMaxMBCount);
    av_log(logctx, AV_LOG_VERBOSE, "min_width: %d, max_width: %d\n",
           caps.nMinWidth, caps.nMaxWidth);
    av_log(logctx, AV_LOG_VERBOSE, "min_height: %d, max_height: %d\n",
           caps.nMinHeight, caps.nMaxHeight);

    if (!caps.bIsSupported) {
        av_log(logctx, AV_LOG_ERROR, "Hardware is lacking required capabilities\n");
        return AVERROR(EINVAL);
    }

    if (params->ulWidth > caps.nMaxWidth || params->ulWidth < caps.nMinWidth) {
        av_log(logctx, AV_LOG_ERROR, "Video width %d not within range from %d to %d\n",
               (int)params->ulWidth, caps.nMinWidth, caps.nMaxWidth);
        return AVERROR(EINVAL);
    }

    if (params->ulHeight > caps.nMaxHeight || params->ulHeight < caps.nMinHeight) {
        av_log(logctx, AV_LOG_ERROR, "Video height %d not within range from %d to %d\n",
               (int)params->ulHeight, caps.nMinHeight, caps.nMaxHeight);
        return AVERROR(EINVAL);
    }

    if ((params->ulWidth * params->ulHeight) / 256 > caps.nMaxMBCount) {
        av_log(logctx, AV_LOG_ERROR, "Video macroblock count %d exceeds maximum of %d\n",
               (int)(params->ulWidth * params->ulHeight) / 256, caps.nMaxMBCount);
        return AVERROR(EINVAL);
    }

    return 0;
}

static void hjkdec_decoder_free(void *opaque, uint8_t *data)
{
    HJKDECDecoder *decoder = (HJKDECDecoder*)data;

    if (decoder->decoder) {
        void *logctx = decoder->hw_device_ref->data;
        HJcontext dummy;
        CHECK_CU(decoder->hjdl->hjCtxPushCurrent(decoder->hjk_ctx));
        CHECK_CU(decoder->hvdl->hjvidDestroyDecoder(decoder->decoder));
        CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));
    }

    av_buffer_unref(&decoder->hw_device_ref);

    hjvid_free_functions(&decoder->hvdl);

    av_freep(&decoder);
}

static int hjkdec_decoder_create(AVBufferRef **out, AVBufferRef *hw_device_ref,
                                HJVIDDECODECREATEINFO *params, void *logctx)
{
    AVHWDeviceContext  *hw_device_ctx = (AVHWDeviceContext*)hw_device_ref->data;
    AVHJKDeviceContext *device_hwctx = hw_device_ctx->hwctx;

    AVBufferRef *decoder_ref;
    HJKDECDecoder *decoder;

    HJcontext dummy;
    int ret;

    decoder = av_mallocz(sizeof(*decoder));
    if (!decoder)
        return AVERROR(ENOMEM);

    decoder_ref = av_buffer_create((uint8_t*)decoder, sizeof(*decoder),
                                   hjkdec_decoder_free, NULL, AV_BUFFER_FLAG_READONLY);
    if (!decoder_ref) {
        av_freep(&decoder);
        return AVERROR(ENOMEM);
    }

    decoder->hw_device_ref = av_buffer_ref(hw_device_ref);
    if (!decoder->hw_device_ref) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    decoder->hjk_ctx = device_hwctx->hjk_ctx;
    decoder->hjdl = device_hwctx->internal->hjk_dl;
    decoder->stream = device_hwctx->stream;

    ret = hjvid_load_functions(&decoder->hvdl, logctx);
    if (ret < 0) {
        av_log(logctx, AV_LOG_ERROR, "Failed loading nvhjvid.\n");
        goto fail;
    }

    ret = CHECK_CU(decoder->hjdl->hjCtxPushCurrent(decoder->hjk_ctx));
    if (ret < 0)
        goto fail;

    ret = hjkdec_test_capabilities(decoder, params, logctx);
    if (ret < 0) {
        CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));
        goto fail;
    }

    ret = CHECK_CU(decoder->hvdl->hjvidCreateDecoder(&decoder->decoder, params));

    CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));

    if (ret < 0) {
        goto fail;
    }

    *out = decoder_ref;

    return 0;
fail:
    av_buffer_unref(&decoder_ref);
    return ret;
}

static AVBufferRef *hjkdec_decoder_frame_alloc(void *opaque, int size)
{
    HJKDECFramePool *pool = opaque;
    AVBufferRef *ret;

    if (pool->nb_allocated >= pool->dpb_size)
        return NULL;

    ret = av_buffer_alloc(sizeof(unsigned int));
    if (!ret)
        return NULL;

    *(unsigned int*)ret->data = pool->nb_allocated++;

    return ret;
}

int ff_hjkdec_decode_uninit(AVCodecContext *avctx)
{
    HJKDECContext *ctx = avctx->internal->hwaccel_priv_data;

    av_freep(&ctx->bitstream);
    ctx->bitstream_len       = 0;
    ctx->bitstream_allocated = 0;

    av_freep(&ctx->slice_offsets);
    ctx->nb_slices               = 0;
    ctx->slice_offsets_allocated = 0;

    av_buffer_unref(&ctx->decoder_ref);
    av_buffer_pool_uninit(&ctx->decoder_pool);

    return 0;
}

int ff_hjkdec_decode_init(AVCodecContext *avctx)
{
    HJKDECContext *ctx = avctx->internal->hwaccel_priv_data;

    printf("%d %s \n", __LINE__, __FUNCTION__);
    HJKDECFramePool      *pool;
    AVHWFramesContext   *frames_ctx;
    const AVPixFmtDescriptor *sw_desc;

    HJVIDDECODECREATEINFO params = { 0 };

    hjkVideoSurfaceFormat output_format;
    int hjvid_codec_type, hjvid_chroma_format, chroma_444;
    int ret = 0;

    sw_desc = av_pix_fmt_desc_get(avctx->sw_pix_fmt);
    if (!sw_desc)
        return AVERROR_BUG;

    hjvid_codec_type = map_avcodec_id(avctx->codec_id);
    if (hjvid_codec_type < 0) {
        av_log(avctx, AV_LOG_ERROR, "Unsupported codec ID\n");
        return AVERROR_BUG;
    }

    hjvid_chroma_format = map_chroma_format(avctx->sw_pix_fmt);
    if (hjvid_chroma_format < 0) {
        av_log(avctx, AV_LOG_ERROR, "Unsupported chroma format\n");
        return AVERROR(ENOSYS);
    }
    chroma_444 = ctx->supports_444 && hjvid_chroma_format == hjkVideoChromaFormat_444;

    if (!avctx->hw_frames_ctx) {
        ret = ff_decode_get_hw_frames_ctx(avctx, AV_HWDEVICE_TYPE_HJK);
        if (ret < 0)
            return ret;
    }

    switch (sw_desc->comp[0].depth) {
    case 8:
        output_format = chroma_444 ? hjkVideoSurfaceFormat_YUV444 :
                                     hjkVideoSurfaceFormat_NV12;
        break;
    case 10:
    case 12:
        output_format = chroma_444 ? hjkVideoSurfaceFormat_YUV444_16Bit :
                                     hjkVideoSurfaceFormat_P016;
        break;
    default:
        av_log(avctx, AV_LOG_ERROR, "Unsupported bit depth\n");
        return AVERROR(ENOSYS);
    }

    frames_ctx = (AVHWFramesContext*)avctx->hw_frames_ctx->data;

    params.ulWidth             = avctx->coded_width;
    params.ulHeight            = avctx->coded_height;
    params.ulTargetWidth       = avctx->coded_width;
    params.ulTargetHeight      = avctx->coded_height;
    params.bitDepthMinus8      = sw_desc->comp[0].depth - 8;
    params.OutputFormat        = output_format;
    params.CodecType           = hjvid_codec_type;
    params.ChromaFormat        = hjvid_chroma_format;
    params.ulNumDecodeSurfaces = frames_ctx->initial_pool_size;
    params.ulNumOutputSurfaces = frames_ctx->initial_pool_size;

    ret = hjkdec_decoder_create(&ctx->decoder_ref, frames_ctx->device_ref, &params, avctx);
    if (ret < 0) {
        if (params.ulNumDecodeSurfaces > 32) {
            av_log(avctx, AV_LOG_WARNING, "Using more than 32 (%d) decode surfaces might cause hjkdec to fail.\n",
                   (int)params.ulNumDecodeSurfaces);
            av_log(avctx, AV_LOG_WARNING, "Try lowering the amount of threads. Using %d right now.\n",
                   avctx->thread_count);
        }
        return ret;
    }

    pool = av_mallocz(sizeof(*pool));
    if (!pool) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    pool->dpb_size = frames_ctx->initial_pool_size;

    ctx->decoder_pool = av_buffer_pool_init2(sizeof(int), pool,
                                             hjkdec_decoder_frame_alloc, av_free);
    if (!ctx->decoder_pool) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    return 0;
fail:
    ff_hjkdec_decode_uninit(avctx);
    return ret;
}

static void hjkdec_fdd_priv_free(void *priv)
{
    HJKDECFrame *cf = priv;

    if (!cf)
        return;

    av_buffer_unref(&cf->idx_ref);
    av_buffer_unref(&cf->decoder_ref);

    av_freep(&priv);
}

static void hjkdec_unmap_mapped_frame(void *opaque, uint8_t *data)
{
    HJKDECFrame *unmap_data = (HJKDECFrame*)data;
    HJKDECDecoder *decoder = (HJKDECDecoder*)unmap_data->decoder_ref->data;
    void *logctx = decoder->hw_device_ref->data;
    HJdeviceptr devptr = (HJdeviceptr)opaque;
    int ret;
    HJcontext dummy;

    ret = CHECK_CU(decoder->hjdl->hjCtxPushCurrent(decoder->hjk_ctx));
    if (ret < 0)
        goto finish;

    CHECK_CU(decoder->hvdl->hjvidUnmapVideoFrame(decoder->decoder, devptr));

    CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));

finish:
    av_buffer_unref(&unmap_data->idx_ref);
    av_buffer_unref(&unmap_data->decoder_ref);
    av_free(unmap_data);
}

static int hjkdec_retrieve_data(void *logctx, AVFrame *frame)
{
    FrameDecodeData  *fdd = (FrameDecodeData*)frame->private_ref->data;
    HJKDECFrame        *cf = (HJKDECFrame*)fdd->hwaccel_priv;
    HJKDECDecoder *decoder = (HJKDECDecoder*)cf->decoder_ref->data;

    AVHWFramesContext *hwctx = (AVHWFramesContext *)frame->hw_frames_ctx->data;

    HJVIDPROCPARAMS vpp = { 0 };
    HJKDECFrame *unmap_data = NULL;

    HJcontext dummy;
    HJdeviceptr devptr;

    unsigned int pitch, i;
    unsigned int offset = 0;
    int shift_h = 0, shift_v = 0;
    int ret = 0;

    vpp.progressive_frame = 1;
    vpp.output_stream = decoder->stream;

    ret = CHECK_CU(decoder->hjdl->hjCtxPushCurrent(decoder->hjk_ctx));
    if (ret < 0)
        return ret;

    ret = CHECK_CU(decoder->hvdl->hjvidMapVideoFrame(decoder->decoder,
                                                     cf->idx, &devptr,
                                                     &pitch, &vpp));
    if (ret < 0)
        goto finish;

    unmap_data = av_mallocz(sizeof(*unmap_data));
    if (!unmap_data) {
        ret = AVERROR(ENOMEM);
        goto copy_fail;
    }

    frame->buf[1] = av_buffer_create((uint8_t *)unmap_data, sizeof(*unmap_data),
                                     hjkdec_unmap_mapped_frame, (void*)devptr,
                                     AV_BUFFER_FLAG_READONLY);
    if (!frame->buf[1]) {
        ret = AVERROR(ENOMEM);
        goto copy_fail;
    }

    unmap_data->idx = cf->idx;
    unmap_data->idx_ref = av_buffer_ref(cf->idx_ref);
    unmap_data->decoder_ref = av_buffer_ref(cf->decoder_ref);

    av_pix_fmt_get_chroma_sub_sample(hwctx->sw_format, &shift_h, &shift_v);
    for (i = 0; frame->linesize[i]; i++) {
        frame->data[i] = (uint8_t*)(devptr + offset);
        frame->linesize[i] = pitch;
        offset += pitch * (frame->height >> (i ? shift_v : 0));
    }

    goto finish;

copy_fail:
    if (!frame->buf[1]) {
        CHECK_CU(decoder->hvdl->hjvidUnmapVideoFrame(decoder->decoder, devptr));
        av_freep(&unmap_data);
    } else {
        av_buffer_unref(&frame->buf[1]);
    }

finish:
    CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));
    return ret;
}

int ff_hjkdec_start_frame(AVCodecContext *avctx, AVFrame *frame)
{
    HJKDECContext *ctx = avctx->internal->hwaccel_priv_data;
    FrameDecodeData *fdd = (FrameDecodeData*)frame->private_ref->data;
    HJKDECFrame *cf = NULL;
    int ret;

    ctx->bitstream_len = 0;
    ctx->nb_slices     = 0;

    if (fdd->hwaccel_priv)
        return 0;

    cf = av_mallocz(sizeof(*cf));
    if (!cf)
        return AVERROR(ENOMEM);

    cf->decoder_ref = av_buffer_ref(ctx->decoder_ref);
    if (!cf->decoder_ref) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    cf->idx_ref = av_buffer_pool_get(ctx->decoder_pool);
    if (!cf->idx_ref) {
        av_log(avctx, AV_LOG_ERROR, "No decoder surfaces left\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    cf->idx = *(unsigned int*)cf->idx_ref->data;

    fdd->hwaccel_priv      = cf;
    fdd->hwaccel_priv_free = hjkdec_fdd_priv_free;
    fdd->post_process      = hjkdec_retrieve_data;

    return 0;
fail:
    hjkdec_fdd_priv_free(cf);
    return ret;

}

int ff_hjkdec_end_frame(AVCodecContext *avctx)
{
    HJKDECContext     *ctx = avctx->internal->hwaccel_priv_data;
    HJKDECDecoder *decoder = (HJKDECDecoder*)ctx->decoder_ref->data;
    void *logctx          = avctx;
    HJVIDPICPARAMS    *pp = &ctx->pic_params;

    HJcontext dummy;

    int ret = 0;

    pp->nBitstreamDataLen = ctx->bitstream_len;
    pp->pBitstreamData    = ctx->bitstream;
    pp->nNumSlices        = ctx->nb_slices;
    pp->pSliceDataOffsets = ctx->slice_offsets;

    ret = CHECK_CU(decoder->hjdl->hjCtxPushCurrent(decoder->hjk_ctx));
    if (ret < 0)
        return ret;

    ret = CHECK_CU(decoder->hvdl->hjvidDecodePicture(decoder->decoder, &ctx->pic_params));
    if (ret < 0)
        goto finish;

finish:
    CHECK_CU(decoder->hjdl->hjCtxPopCurrent(&dummy));

    return ret;
}

int ff_hjkdec_simple_end_frame(AVCodecContext *avctx)
{
    HJKDECContext *ctx = avctx->internal->hwaccel_priv_data;
    int ret = ff_hjkdec_end_frame(avctx);
    ctx->bitstream = NULL;
    return ret;
}

int ff_hjkdec_simple_decode_slice(AVCodecContext *avctx, const uint8_t *buffer,
                                 uint32_t size)
{
    HJKDECContext *ctx = avctx->internal->hwaccel_priv_data;
    void *tmp;

    tmp = av_fast_realloc(ctx->slice_offsets, &ctx->slice_offsets_allocated,
                          (ctx->nb_slices + 1) * sizeof(*ctx->slice_offsets));
    if (!tmp)
        return AVERROR(ENOMEM);
    ctx->slice_offsets = tmp;

    if (!ctx->bitstream)
        ctx->bitstream = (uint8_t*)buffer;

    ctx->slice_offsets[ctx->nb_slices] = buffer - ctx->bitstream;
    ctx->bitstream_len += size;
    ctx->nb_slices++;

    return 0;
}

static void hjkdec_free_dummy(struct AVHWFramesContext *ctx)
{
    av_buffer_pool_uninit(&ctx->pool);
}

static AVBufferRef *hjkdec_alloc_dummy(int size)
{
    return av_buffer_create(NULL, 0, NULL, NULL, 0);
}

int ff_hjkdec_frame_params(AVCodecContext *avctx,
                          AVBufferRef *hw_frames_ctx,
                          int dpb_size,
                          int supports_444)
{
    AVHWFramesContext *frames_ctx = (AVHWFramesContext*)hw_frames_ctx->data;
    const AVPixFmtDescriptor *sw_desc;
    int hjvid_codec_type, hjvid_chroma_format, chroma_444;

    sw_desc = av_pix_fmt_desc_get(avctx->sw_pix_fmt);
    if (!sw_desc)
        return AVERROR_BUG;

    hjvid_codec_type = map_avcodec_id(avctx->codec_id);
    if (hjvid_codec_type < 0) {
        av_log(avctx, AV_LOG_ERROR, "Unsupported codec ID\n");
        return AVERROR_BUG;
    }

    hjvid_chroma_format = map_chroma_format(avctx->sw_pix_fmt);
    if (hjvid_chroma_format < 0) {
        av_log(avctx, AV_LOG_VERBOSE, "Unsupported chroma format\n");
        return AVERROR(EINVAL);
    }
    chroma_444 = supports_444 && hjvid_chroma_format == hjkVideoChromaFormat_444;

    frames_ctx->format            = AV_PIX_FMT_HJK;
    frames_ctx->width             = (avctx->coded_width + 1) & ~1;
    frames_ctx->height            = (avctx->coded_height + 1) & ~1;
    /*
     * We add two extra frames to the pool to account for deinterlacing filters
     * holding onto their frames.
     */
    frames_ctx->initial_pool_size = dpb_size + 2;

    frames_ctx->free = hjkdec_free_dummy;
    frames_ctx->pool = av_buffer_pool_init(0, hjkdec_alloc_dummy);

    if (!frames_ctx->pool)
        return AVERROR(ENOMEM);

    switch (sw_desc->comp[0].depth) {
    case 8:
        frames_ctx->sw_format = chroma_444 ? AV_PIX_FMT_YUV444P : AV_PIX_FMT_NV12;
        break;
    case 10:
        frames_ctx->sw_format = chroma_444 ? AV_PIX_FMT_YUV444P16 : AV_PIX_FMT_P010;
        break;
    case 12:
        frames_ctx->sw_format = chroma_444 ? AV_PIX_FMT_YUV444P16 : AV_PIX_FMT_P016;
        break;
    default:
        return AVERROR(EINVAL);
    }

    return 0;
}

int ff_hjkdec_get_ref_idx(AVFrame *frame)
{
    FrameDecodeData *fdd;
    HJKDECFrame *cf;

    if (!frame || !frame->private_ref)
        return -1;

    fdd = (FrameDecodeData*)frame->private_ref->data;
    cf  = (HJKDECFrame*)fdd->hwaccel_priv;
    if (!cf)
        return -1;

    return cf->idx;
}
