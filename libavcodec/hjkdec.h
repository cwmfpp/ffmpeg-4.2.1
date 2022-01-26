/*
 * HW decode acceleration through HJKDEC
 *
 * Copyright (c) 2022
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

#ifndef AVCODEC_HJKDEC_H
#define AVCODEC_HJKDEC_H

#ifdef ABCD
#include "compat/hjk/dynlink_loader.h"
#endif
#include "ffhjkcodec/hjk_api.h"

#include <stdint.h>

#include "libavutil/buffer.h"
#include "libavutil/frame.h"

#include "avcodec.h"

#if defined(HJKDECAPI_MAJOR_VERSION) && defined(HJKDECAPI_MINOR_VERSION)
# define HJKDECAPI_CHECK_VERSION(major, minor) \
    ((major) < HJKDECAPI_MAJOR_VERSION || ((major) == HJKDECAPI_MAJOR_VERSION && (minor) <= HJKDECAPI_MINOR_VERSION))
#else
/* version macros were added in SDK 8.1 ffhjkcodec */
# define HJKDECAPI_CHECK_VERSION(major, minor) \
    ((major) < 8 || ((major) == 8 && (minor) <= 0))
#endif

typedef struct HJKDECFrame {
    unsigned int idx;/* map decode data index */
    AVBufferRef *idx_ref;
    AVBufferRef *decoder_ref;
} HJKDECFrame;

typedef struct HJKDECContext {
    HJVIDPICPARAMS pic_params;

    AVBufferPool *decoder_pool;

    AVBufferRef  *decoder_ref;

    uint8_t      *bitstream;
    int           bitstream_len;
    unsigned int  bitstream_allocated;

    unsigned     *slice_offsets;
    int           nb_slices;
    unsigned int  slice_offsets_allocated;

    int           supports_444;
} HJKDECContext;

int ff_hjkdec_decode_init(AVCodecContext *avctx);
int ff_hjkdec_decode_uninit(AVCodecContext *avctx);
int ff_hjkdec_start_frame(AVCodecContext *avctx, AVFrame *frame);
int ff_hjkdec_end_frame(AVCodecContext *avctx);
int ff_hjkdec_simple_end_frame(AVCodecContext *avctx);
int ff_hjkdec_simple_decode_slice(AVCodecContext *avctx, const uint8_t *buffer,
                                 uint32_t size);
int ff_hjkdec_frame_params(AVCodecContext *avctx,
                          AVBufferRef *hw_frames_ctx,
                          int dpb_size,
                          int supports_444);
int ff_hjkdec_get_ref_idx(AVFrame *frame);

#endif /* AVCODEC_HJKDEC_H */
