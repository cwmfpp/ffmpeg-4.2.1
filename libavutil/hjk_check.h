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


#ifndef AVUTIL_HJK_CHECK_H
#define AVUTIL_HJK_CHECK_H

typedef HJresult HJKAPI hjk_check_GetErrorName(HJresult error, const char** pstr);
typedef HJresult HJKAPI hjk_check_GetErrorString(HJresult error, const char** pstr);

/**
 * Wrap a HJK function call and print error information if it fails.
 */
static inline int ff_hjk_check(void *avctx,
                                void *hjGetErrorName_fn, void *hjGetErrorString_fn,
                                HJresult err, const char *func)
{
    const char *err_name;
    const char *err_string;

    av_log(avctx, AV_LOG_TRACE, "Calling %s\n", func);

    if (err == HJK_SUCCESS)
        return 0;

    ((hjk_check_GetErrorName *)hjGetErrorName_fn)(err, &err_name);
    ((hjk_check_GetErrorString *)hjGetErrorString_fn)(err, &err_string);

    av_log(avctx, AV_LOG_ERROR, "%s failed", func);
    if (err_name && err_string)
        av_log(avctx, AV_LOG_ERROR, " -> %s: %s", err_name, err_string);
    av_log(avctx, AV_LOG_ERROR, "\n");

    return AVERROR_EXTERNAL;
}

/**
 * Convenience wrapper for ff_hjk_check when directly linking libhjk.
 */

#define FF_HJK_CHECK(avclass, x) ff_hjk_check(avclass, hjGetErrorName, hjGetErrorString, (x), #x)

/**
 * Convenience wrapper for ff_hjk_check when dynamically loading hjk symbols.
 */

#define FF_HJK_CHECK_DL(avclass, hjdl, x) ff_hjk_check(avclass, hjdl->hjGetErrorName, hjdl->hjGetErrorString, (x), #x)

#endif /* AVUTIL_HJK_CHECK_H */
