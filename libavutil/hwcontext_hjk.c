/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTIHJLAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "buffer.h"
#include "common.h"
#include "hwcontext.h"
#include "hwcontext_internal.h"
#include "hwcontext_hjk_internal.h"
#include "hjk_check.h"
#include "mem.h"
#include "pixdesc.h"
#include "pixfmt.h"
#include "imgutils.h"

#define HJK_FRAME_ALIGNMENT 128

typedef struct HJKFramesContext {
    int shift_width, shift_height;
} HJKFramesContext;

static const enum AVPixelFormat supported_formats[] = {
    AV_PIX_FMT_NV12,
    AV_PIX_FMT_YUV420P,
    AV_PIX_FMT_YUV444P,
    AV_PIX_FMT_P010,
    AV_PIX_FMT_P016,
    AV_PIX_FMT_YUV444P16,
    AV_PIX_FMT_0RGB32,
    AV_PIX_FMT_0BGR32,
};

#define CHECK_HJ(x) FF_HJK_CHECK_DL(device_ctx, hj, x)

static int hjk_frames_get_constraints(AVHWDeviceContext *ctx,
                                       const void *hwconfig,
                                       AVHWFramesConstraints *constraints)
{
    int i;

    constraints->valid_sw_formats = av_malloc_array(FF_ARRAY_ELEMS(supported_formats) + 1,
                                                    sizeof(*constraints->valid_sw_formats));
    if (!constraints->valid_sw_formats)
        return AVERROR(ENOMEM);

    for (i = 0; i < FF_ARRAY_ELEMS(supported_formats); i++)
        constraints->valid_sw_formats[i] = supported_formats[i];
    constraints->valid_sw_formats[FF_ARRAY_ELEMS(supported_formats)] = AV_PIX_FMT_NONE;

    constraints->valid_hw_formats = av_malloc_array(2, sizeof(*constraints->valid_hw_formats));
    if (!constraints->valid_hw_formats)
        return AVERROR(ENOMEM);

    constraints->valid_hw_formats[0] = AV_PIX_FMT_HJK;
    constraints->valid_hw_formats[1] = AV_PIX_FMT_NONE;

    return 0;
}

static void hjk_buffer_free(void *opaque, uint8_t *data)
{
    AVHWFramesContext        *ctx = opaque;
    AVHWDeviceContext *device_ctx = ctx->device_ctx;
    AVHJKDeviceContext    *hwctx = device_ctx->hwctx;
    HjkFunctions             *hj = hwctx->internal->hjk_dl;

    HJcontext dummy;

    CHECK_HJ(hj->hjCtxPushCurrent(hwctx->hjk_ctx));

    CHECK_HJ(hj->hjMemFree((HJdeviceptr)data));

    CHECK_HJ(hj->hjCtxPopCurrent(&dummy));
}

static AVBufferRef *hjk_pool_alloc(void *opaque, int size)
{
    AVHWFramesContext        *ctx = opaque;
    AVHWDeviceContext *device_ctx = ctx->device_ctx;
    AVHJKDeviceContext    *hwctx = device_ctx->hwctx;
    HjkFunctions             *hj = hwctx->internal->hjk_dl;

    AVBufferRef *ret = NULL;
    HJcontext dummy = NULL;
    HJdeviceptr data;
    int err;

    err = CHECK_HJ(hj->hjCtxPushCurrent(hwctx->hjk_ctx));
    if (err < 0)
        return NULL;

    err = CHECK_HJ(hj->hjMemAlloc(&data, size));
    if (err < 0)
        goto fail;

    ret = av_buffer_create((uint8_t*)data, size, hjk_buffer_free, ctx, 0);
    if (!ret) {
        CHECK_HJ(hj->hjMemFree(data));
        goto fail;
    }

fail:
    CHECK_HJ(hj->hjCtxPopCurrent(&dummy));
    return ret;
}

static int hjk_frames_init(AVHWFramesContext *ctx)
{
    HJKFramesContext *priv = ctx->internal->priv;
    int i;

    for (i = 0; i < FF_ARRAY_ELEMS(supported_formats); i++) {
        if (ctx->sw_format == supported_formats[i])
            break;
    }
    if (i == FF_ARRAY_ELEMS(supported_formats)) {
        av_log(ctx, AV_LOG_ERROR, "Pixel format '%s' is not supported\n",
               av_get_pix_fmt_name(ctx->sw_format));
        return AVERROR(ENOSYS);
    }

    av_pix_fmt_get_chroma_sub_sample(ctx->sw_format, &priv->shift_width, &priv->shift_height);

    if (!ctx->pool) {
        int size = av_image_get_buffer_size(ctx->sw_format, ctx->width, ctx->height, HJK_FRAME_ALIGNMENT);
        if (size < 0)
            return size;

        ctx->internal->pool_internal = av_buffer_pool_init2(size, ctx, hjk_pool_alloc, NULL);
        if (!ctx->internal->pool_internal)
            return AVERROR(ENOMEM);
    }

    return 0;
}

static int hjk_get_buffer(AVHWFramesContext *ctx, AVFrame *frame)
{
    int res;

    frame->buf[0] = av_buffer_pool_get(ctx->pool);
    if (!frame->buf[0])
        return AVERROR(ENOMEM);

    res = av_image_fill_arrays(frame->data, frame->linesize, frame->buf[0]->data,
                               ctx->sw_format, ctx->width, ctx->height, HJK_FRAME_ALIGNMENT);
    if (res < 0)
        return res;

    // YUV420P is a special case.
    // Hjenc expects the U/V planes in swapped order from how ffmpeg expects them, also chroma is half-aligned
    if (ctx->sw_format == AV_PIX_FMT_YUV420P) {
        frame->linesize[1] = frame->linesize[2] = frame->linesize[0] / 2;
        frame->data[2]     = frame->data[1];
        frame->data[1]     = frame->data[2] + frame->linesize[2] * ctx->height / 2;
    }

    frame->format = AV_PIX_FMT_HJK;
    frame->width  = ctx->width;
    frame->height = ctx->height;

    return 0;
}

static int hjk_transfer_get_formats(AVHWFramesContext *ctx,
                                     enum AVHWFrameTransferDirection dir,
                                     enum AVPixelFormat **formats)
{
    enum AVPixelFormat *fmts;

    fmts = av_malloc_array(2, sizeof(*fmts));
    if (!fmts)
        return AVERROR(ENOMEM);

    fmts[0] = ctx->sw_format;
    fmts[1] = AV_PIX_FMT_NONE;

    *formats = fmts;

    return 0;
}

static int hjk_transfer_data_from(AVHWFramesContext *ctx, AVFrame *dst,
                                   const AVFrame *src)
{
    HJKFramesContext       *priv = ctx->internal->priv;
    AVHWDeviceContext *device_ctx = ctx->device_ctx;
    AVHJKDeviceContext    *hwctx = device_ctx->hwctx;
    HjkFunctions             *hj = hwctx->internal->hjk_dl;

    HJcontext dummy;
    int i, ret;

    ret = CHECK_HJ(hj->hjCtxPushCurrent(hwctx->hjk_ctx));
    if (ret < 0)
        return ret;

    for (i = 0; i < FF_ARRAY_ELEMS(src->data) && src->data[i]; i++) {
        HJK_MEMCPY2D cpy = {
            .srcMemoryType = HJ_MEMORYTYPE_DEVICE,
            .dstMemoryType = HJ_MEMORYTYPE_HOST,
            .srcDevice     = (HJdeviceptr)src->data[i],
            .dstHost       = dst->data[i],
            .srcPitch      = src->linesize[i],
            .dstPitch      = dst->linesize[i],
            .WidthInBytes  = FFMIN(src->linesize[i], dst->linesize[i]),
            .Height        = src->height >> (i ? priv->shift_height : 0),
        };

        ret = CHECK_HJ(hj->hjMemcpy2DAsync(&cpy, hwctx->stream));
        if (ret < 0)
            goto exit;
    }

    ret = CHECK_HJ(hj->hjStreamSynchronize(hwctx->stream));
    if (ret < 0)
        goto exit;

exit:
    CHECK_HJ(hj->hjCtxPopCurrent(&dummy));

    return 0;
}

static int hjk_transfer_data_to(AVHWFramesContext *ctx, AVFrame *dst,
                                 const AVFrame *src)
{
    HJKFramesContext       *priv = ctx->internal->priv;
    AVHWDeviceContext *device_ctx = ctx->device_ctx;
    AVHJKDeviceContext    *hwctx = device_ctx->hwctx;
    HjkFunctions             *hj = hwctx->internal->hjk_dl;

    HJcontext dummy;
    int i, ret;

    ret = CHECK_HJ(hj->hjCtxPushCurrent(hwctx->hjk_ctx));
    if (ret < 0)
        return ret;

    for (i = 0; i < FF_ARRAY_ELEMS(src->data) && src->data[i]; i++) {
        HJK_MEMCPY2D cpy = {
            .srcMemoryType = HJ_MEMORYTYPE_HOST,
            .dstMemoryType = HJ_MEMORYTYPE_DEVICE,
            .srcHost       = src->data[i],
            .dstDevice     = (HJdeviceptr)dst->data[i],
            .srcPitch      = src->linesize[i],
            .dstPitch      = dst->linesize[i],
            .WidthInBytes  = FFMIN(src->linesize[i], dst->linesize[i]),
            .Height        = src->height >> (i ? priv->shift_height : 0),
        };

        ret = CHECK_HJ(hj->hjMemcpy2DAsync(&cpy, hwctx->stream));
        if (ret < 0)
            goto exit;
    }

exit:
    CHECK_HJ(hj->hjCtxPopCurrent(&dummy));

    return 0;
}

static void hjk_device_uninit(AVHWDeviceContext *device_ctx)
{
    AVHJKDeviceContext *hwctx = device_ctx->hwctx;

    if (hwctx->internal) {
        HjkFunctions *hj = hwctx->internal->hjk_dl;
        if (hwctx->internal->is_allocated && hwctx->hjk_ctx) {
            CHECK_HJ(hj->hjCtxDestroy(hwctx->hjk_ctx));
            hwctx->hjk_ctx = NULL;
        }
        hjk_free_functions(&hwctx->internal->hjk_dl);
    }

    av_freep(&hwctx->internal);
}

static int hjk_device_init(AVHWDeviceContext *ctx)
{
    AVHJKDeviceContext *hwctx = ctx->hwctx;
    int ret;

    if (!hwctx->internal) {
        hwctx->internal = av_mallocz(sizeof(*hwctx->internal));
        if (!hwctx->internal)
            return AVERROR(ENOMEM);
    }

    if (!hwctx->internal->hjk_dl) {
        ret = hjk_load_functions(&hwctx->internal->hjk_dl, ctx);
        if (ret < 0) {
            av_log(ctx, AV_LOG_ERROR, "Could not dynamically load HJK\n");
            goto error;
        }
    }

    return 0;

error:
    hjk_device_uninit(ctx);
    return ret;
}

static int hjk_device_create(AVHWDeviceContext *device_ctx,
                              const char *device,
                              AVDictionary *opts, int flags)
{
    AVHJKDeviceContext *hwctx = device_ctx->hwctx;
    HjkFunctions *hj;
    HJdevice hj_device;
    HJcontext dummy;
    int ret, device_idx = 0;

    if (device)
        device_idx = strtol(device, NULL, 0);

    if (hjk_device_init(device_ctx) < 0)
        goto error;

    hj = hwctx->internal->hjk_dl;

    ret = CHECK_HJ(hj->hjInit(0));
    if (ret < 0)
        goto error;

    ret = CHECK_HJ(hj->hjDeviceGet(&hj_device, device_idx));
    if (ret < 0)
        goto error;

    ret = CHECK_HJ(hj->hjCtxCreate(&hwctx->hjk_ctx, HJ_CTX_SCHED_BLOCKING_SYNC, hj_device));
    if (ret < 0)
        goto error;

    // Setting stream to NULL will make functions automatically use the default HJstream
    hwctx->stream = NULL;

    CHECK_HJ(hj->hjCtxPopCurrent(&dummy));

    hwctx->internal->is_allocated = 1;

    return 0;

error:
    hjk_device_uninit(device_ctx);
    return AVERROR_UNKNOWN;
}

const HWContextType ff_hwcontext_type_hjk = {
    .type                 = AV_HWDEVICE_TYPE_HJK,
    .name                 = "HJK",

    .device_hwctx_size    = sizeof(AVHJKDeviceContext),
    .frames_priv_size     = sizeof(HJKFramesContext),

    .device_create        = hjk_device_create,
    .device_init          = hjk_device_init,
    .device_uninit        = hjk_device_uninit,
    .frames_get_constraints = hjk_frames_get_constraints,
    .frames_init          = hjk_frames_init,
    .frames_get_buffer    = hjk_get_buffer,
    .transfer_get_formats = hjk_transfer_get_formats,
    .transfer_data_to     = hjk_transfer_data_to,
    .transfer_data_from   = hjk_transfer_data_from,

    .pix_fmts             = (const enum AVPixelFormat[]){ AV_PIX_FMT_HJK, AV_PIX_FMT_NONE },
};
