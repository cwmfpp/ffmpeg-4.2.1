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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_HJKENC_H
#define AVCODEC_HJKENC_H

#include "config.h"

#if CONFIG_D3D11VA
#define COBJMACROS
#include "libavutil/hwcontext_d3d11va.h"
#else
typedef void ID3D11Device;
#endif

#ifdef ABCD
#include <ffhjkcodec/hjkEncodeAPI.h>

#include "compat/hjk/dynlink_loader.h"
#endif

#include "libavutil/fifo.h"
#include "libavutil/opt.h"

#include "avcodec.h"
#include "hjk_api.h"

#define MAX_REGISTERED_FRAMES 64
#define RC_MODE_DEPRECATED 0x800000
#define RCD(rc_mode) ((rc_mode) | RC_MODE_DEPRECATED)

#define HJKENCAPI_CHECK_VERSION(major, minor) \
    ((major) < HJKENCAPI_MAJOR_VERSION || ((major) == HJKENCAPI_MAJOR_VERSION && (minor) <= HJKENCAPI_MINOR_VERSION))

// SDK 8.1 compile time feature checks
#if HJKENCAPI_CHECK_VERSION(8, 1)
#define HJKENC_HAVE_BFRAME_REF_MODE
#define HJKENC_HAVE_QP_MAP_MODE
#endif

// SDK 9.0 compile time feature checks
#if HJKENCAPI_CHECK_VERSION(9, 0)
#define HJKENC_HAVE_HEVC_BFRAME_REF_MODE
#endif

typedef struct HjkencSurface
{
    HJK_ENC_INPUT_PTR input_surface;
    AVFrame *in_ref;
    int reg_idx;
    int width;
    int height;
    int pitch;

    HJK_ENC_OUTPUT_PTR output_surface; /* decode out, point to internal info */
    HJK_ENC_BUFFER_FORMAT format;
    int size;
} HjkencSurface;

typedef struct HjkencDynLoadFunctions
{
    HjkFunctions *hjk_dl;
    HjkencFunctions *hjkenc_dl;

    HJK_ENCODE_API_FUNCTION_LIST hjkenc_funcs;
    int hjkenc_device_count;
} HjkencDynLoadFunctions;

enum {
    PRESET_DEFAULT = 0,
    PRESET_SLOW,
    PRESET_MEDIUM,
    PRESET_FAST,
    PRESET_HP,
    PRESET_HQ,
    PRESET_BD ,
    PRESET_LOW_LATENCY_DEFAULT ,
    PRESET_LOW_LATENCY_HQ ,
    PRESET_LOW_LATENCY_HP,
    PRESET_LOSSLESS_DEFAULT, // lossless presets must be the last ones
    PRESET_LOSSLESS_HP,
};

enum {
    HJK_ENC_MJPEG_PROFILE_BASELINE,
    HJK_ENC_MJPEG_PROFILE_MAIN,
    HJK_ENC_MJPEG_PROFILE_HIGH,
    HJK_ENC_MJPEG_PROFILE_HIGH_444P,
};

enum {
    HJK_ENC_HEVC_PROFILE_MAIN,
    HJK_ENC_HEVC_PROFILE_MAIN_10,
    HJK_ENC_HEVC_PROFILE_REXT,
};

enum {
    HJKENC_LOWLATENCY = 1,
    HJKENC_LOSSLESS   = 2,
    HJKENC_ONE_PASS   = 4,
    HJKENC_TWO_PASSES = 8,
};

enum {
    LIST_DEVICES = -2,
    ANY_DEVICE,
};

typedef struct HjkencContext
{
    AVClass *avclass;

    HjkencDynLoadFunctions hjkenc_dload_funcs;

    HJK_ENC_INITIALIZE_PARAMS init_encode_params;
    HJK_ENC_CONFIG encode_config;
    HJcontext hjk_context;
    HJcontext hjk_context_internal;
    ID3D11Device *d3d11_device;

    int nb_surfaces;
    HjkencSurface *surfaces;

    AVFifoBuffer *unused_surface_queue;
    AVFifoBuffer *output_surface_queue;
    AVFifoBuffer *output_surface_ready_queue;
    AVFifoBuffer *timestamp_list;

    int encoder_flushing;

    struct {
        void *ptr; /* = frame->data[0] */
        int ptr_index;
        HJK_ENC_REGISTERED_PTR regptr; /* point to internal register HJK_ENC_REGISTER_RESOURCE*/
        int mapped;
        HJK_ENC_MAP_INPUT_RESOURCE in_map;
    } registered_frames[MAX_REGISTERED_FRAMES];
    int nb_registered_frames;

    /* the actual data pixel format, different from
     * AVCodecContext.pix_fmt when using hwaccel frames on input */
    enum AVPixelFormat data_pix_fmt;

    /* timestamps of the first two frames, for computing the first dts
     * when B-frames are present */
    int64_t initial_pts[2];
    int first_packet_output;

    int support_dyn_bitrate;

    void *hjkencoder;

    int preset;
    int profile;
    int level;
    int tier;
    int rc;
    int cbr;
    int twopass;
    int device;
    int flags;
    int async_depth;
    int rc_lookahead;
    int aq;
    int no_scenecut;
    int forced_idr;
    int b_adapt;
    int temporal_aq;
    int zerolatency;
    int nonref_p;
    int strict_gop;
    int aq_strength;
    float quality;
    int aud;
    int bluray_compat;
    int init_qp_p;
    int init_qp_b;
    int init_qp_i;
    int cqp;
    int weighted_pred;
    int coder;
    int b_ref_mode;
    int a53_cc;
} HjkencContext;

int ff_hjkenc_encode_init(AVCodecContext *avctx);

int ff_hjkenc_encode_close(AVCodecContext *avctx);

int ff_hjkenc_send_frame(AVCodecContext *avctx, const AVFrame *frame);

int ff_hjkenc_receive_packet(AVCodecContext *avctx, AVPacket *pkt);

int ff_hjkenc_encode_frame(AVCodecContext *avctx, AVPacket *pkt,
                          const AVFrame *frame, int *got_packet);

extern const enum AVPixelFormat ff_hjkenc_pix_fmts[];

#endif /* AVCODEC_HJKENC_H */
