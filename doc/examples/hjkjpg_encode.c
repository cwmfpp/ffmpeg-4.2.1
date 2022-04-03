/*
 * Video Acceleration API (video encoding) encode sample
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * Intel HJKJPG-accelerated encoding example.
 *
 * @example hjkjpg_encode.c
 * This example shows how to do HJKJPG-accelerated encoding. now only support NV12
 * raw file, usage like: hjkjpg_encode 1920 1080 input.yuv output.h264
 *
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>

#include <libavcodec/avcodec.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>

static int g_run_status = 0;

static int set_hwframe_ctx(AVCodecContext *ctx, AVBufferRef *hw_device_ctx, int width, int height, enum AVPixelFormat sw_format)
{
    AVBufferRef *hw_frames_ref;
    AVHWFramesContext *frames_ctx = NULL;
    int err = 0;

    if (!(hw_frames_ref = av_hwframe_ctx_alloc(hw_device_ctx))) {
        fprintf(stderr, "Failed to create HJK frame context.\n");
        return -1;
    }
    frames_ctx = (AVHWFramesContext *)(hw_frames_ref->data);
    frames_ctx->format    = AV_PIX_FMT_HJK;
    frames_ctx->sw_format = sw_format;
    frames_ctx->width     = width;
    frames_ctx->height    = height;
    frames_ctx->initial_pool_size = 20;
    if ((err = av_hwframe_ctx_init(hw_frames_ref)) < 0) {
        fprintf(stderr, "Failed to initialize HJK frame context."
                "Error code: %s\n",av_err2str(err));
        av_buffer_unref(&hw_frames_ref);
        return err;
    }
    ctx->hw_frames_ctx = av_buffer_ref(hw_frames_ref);
    if (!ctx->hw_frames_ctx)
        err = AVERROR(ENOMEM);

    av_buffer_unref(&hw_frames_ref);
    return err;
}

struct thread_info {     /* Used as argument to thread_encoder() */
    pthread_t thread_id; /* ID returned by pthread_create() */
    int thread_count;
    int thread_index;      /* Application-defined thread # */
    char m_yuv_name[128];
    char m_jpg_name[128];
    int m_width;
    int m_height;
    enum AVPixelFormat m_sw_format;
    int m_resize_width;
    int m_resize_height;
    int m_read_cycle;
    int m_read_yuv_num;
    int m_encoder_jpeg_num;
};

static int encode_write(AVCodecContext *avctx, AVFrame *frame, FILE *fout, struct thread_info *tinfo)
{
    int ret = 0;
    AVPacket enc_pkt;

    av_init_packet(&enc_pkt);
    enc_pkt.data = NULL;
    enc_pkt.size = 0;

    if ((ret = avcodec_send_frame(avctx, frame)) < 0) {
        fprintf(stderr, "Error code: %s\n", av_err2str(ret));
        goto end;
    }
    while (1) {
        ret = avcodec_receive_packet(avctx, &enc_pkt);
        if (ret) {
            //printf("%d %s ret=%d\n", __LINE__, __FUNCTION__, ret);
            break;
        }

        tinfo->m_encoder_jpeg_num++;
        enc_pkt.stream_index = 0;
        if (NULL != fout) {
            ret = fwrite(enc_pkt.data, enc_pkt.size, 1, fout);
        }
        av_packet_unref(&enc_pkt);
    }
    
    if (ret == AVERROR(EAGAIN)) {
        //printf("%d %s ret=%d eagain\n", __LINE__, __FUNCTION__, ret);
    }
end:
    ret = ((ret == AVERROR(EAGAIN)) ? 0 : -1);
    return ret;
}

static void *thread_encoder(void *arg) {
    struct thread_info *tinfo = arg;
    char *uargv = NULL;
    char *p = NULL;

    printf("Thread %d: top of stack near %p; bottom %p\n", tinfo->thread_index,
           &p, &uargv);

    int size, err;
    FILE *fin = NULL, *fout = NULL;
    AVFrame *hw_frame = NULL;
    AVCodecContext *avctx = NULL;
    AVCodec *codec = NULL;
    const char *enc_name = "mjpeg_hjkenc";
    int width, height; /*todo*/
    enum AVPixelFormat sw_format = AV_PIX_FMT_NONE;
    AVBufferRef *hw_device_ctx = NULL; /*todo*/

    width  = tinfo->m_width;
    height = tinfo->m_height;
    size   = width * height;
    sw_format = tinfo->m_sw_format; 

    if (!(fin = fopen(tinfo->m_yuv_name, "r"))) {
        fprintf(stderr, "Fail to open input file[%s] : %s\n", tinfo->m_yuv_name, strerror(errno));
        return NULL;
    }
    if (strlen(tinfo->m_jpg_name) > 0 && !(fout = fopen(tinfo->m_jpg_name, "w+b"))) {
        fprintf(stderr, "Fail to open output file : %s\n", strerror(errno));
        err = -1;
        goto close;
    }

    err = av_hwdevice_ctx_create(&hw_device_ctx, AV_HWDEVICE_TYPE_HJK,
                                 NULL, NULL, 0);
    if (err < 0) {
        fprintf(stderr, "Failed to create a HJK device. Error code: %s\n", av_err2str(err));
        goto close;
    }

    if (!(codec = avcodec_find_encoder_by_name(enc_name))) {
        fprintf(stderr, "Could not find encoder.\n");
        err = -1;
        goto close;
    }

    if (!(avctx = avcodec_alloc_context3(codec))) {
        err = AVERROR(ENOMEM);
        goto close;
    }

    avctx->width     = width;
    avctx->height    = height;
    avctx->time_base = (AVRational){1, 25};
    avctx->framerate = (AVRational){25, 1};
    avctx->sample_aspect_ratio = (AVRational){1, 1};
    avctx->pix_fmt   = AV_PIX_FMT_HJK;

    /* set hw_frames_ctx for encoder's AVCodecContext */
    if ((err = set_hwframe_ctx(avctx, hw_device_ctx, width, height, sw_format)) < 0) {
        fprintf(stderr, "Failed to set hwframe context.\n");
        goto close;
    }

    if ((err = avcodec_open2(avctx, codec, NULL)) < 0) {
        fprintf(stderr, "Cannot open video encoder codec. Error code: %s\n", av_err2str(err));
        goto close;
    }

    while (1) {
        if (1 == g_run_status) {
            break;
        }
        if (!(hw_frame = av_frame_alloc())) {
            err = AVERROR(ENOMEM);
            goto close;
        }
        if ((err = av_hwframe_get_buffer(avctx->hw_frames_ctx, hw_frame, 0)) < 0) {
            fprintf(stderr, "Error code: %s.\n", av_err2str(err));
            goto close;
        }
        if (!hw_frame->hw_frames_ctx) {
            err = AVERROR(ENOMEM);
            goto close;
        }
        if (AV_PIX_FMT_YUYV422 == sw_format) {
            if ((err = fread((uint8_t *)(hw_frame->data[0]), size * 2, 1,
                             fin)) <= 0) {
                if (1 == tinfo->m_read_cycle && feof(fin)) {
                    if (0 != fseek(fin, 0L, SEEK_SET)) {
                        break;
                    }
                    if ((err = fread((uint8_t *)(hw_frame->data[0]), size * 2,
                                     1, fin)) <= 0) {
                        break;
                    }
                } else {
                    break;
                }
            }
        } else if (AV_PIX_FMT_NV12 == sw_format) {

            if ((err = fread((uint8_t *)(hw_frame->data[0]), size, 1,
                             fin)) <= 0) {
                if (1 == tinfo->m_read_cycle && feof(fin)) {
                    if (0 != fseek(fin, 0L, SEEK_SET)) {
                        break;
                    }
                    if ((err = fread((uint8_t *)(hw_frame->data[0]), size,
                                     1, fin)) <= 0) {
                        break;
                    }
                } else {
                    break;
                }
            }

            if ((err = fread((uint8_t *)(hw_frame->data[1]), size / 2, 1, fin)) <=
                0) {
                if (1 == tinfo->m_read_cycle && feof(fin)) {
                    if (0 != fseek(fin, 0L, SEEK_SET)) {
                        break;
                    }
                    if ((err = fread((uint8_t *)(hw_frame->data[0]), size / 2, 1,
                                     fin)) <= 0) {
                        break;
                    }
                } else {
                    break;
                }
            }
        }

        tinfo->m_read_yuv_num++;

        if ((err = (encode_write(avctx, hw_frame, fout, tinfo))) < 0) {
            fprintf(stderr, "Failed to encode.\n");
            goto close;
        }
        av_frame_free(&hw_frame);
    }

    /* flush encoder */
    err = encode_write(avctx, NULL, fout, tinfo);
    if (err == AVERROR_EOF)
        err = 0;

close:
    if (fin)
        fclose(fin);
    if (fout)
        fclose(fout);
    av_frame_free(&hw_frame);
    avcodec_free_context(&avctx);
    av_buffer_unref(&hw_device_ctx);

    return uargv;
}

static void* thread_encode_statics(void *arg)
{
    uint64_t last_count_sum = 0;
    uint64_t count_sum = 0;
    uint64_t run_time_sum = 0;
    uint64_t frame_sum = 0;
    char hjk_encoder_buf[1024];

    struct thread_info *tinfo = arg;
    #define INTERVAL 1

    for (;;){
        if (2 == g_run_status) {
            g_run_status = 3;
            break;
        }

        sleep(INTERVAL);

        count_sum = 0;
        for (int i = 0; i < tinfo[0].thread_count; i++) {
            count_sum += tinfo[i].m_encoder_jpeg_num;
        }
        if (count_sum - last_count_sum > 0) {
            run_time_sum += INTERVAL;
            frame_sum = frame_sum + (count_sum - last_count_sum);
        }
        printf("thread sum %d, frame %d, fcurr fps %2.2f, avg fps %2.2f, run "
               "time sum %ds",
               tinfo[0].thread_count, count_sum,
               ((double)(count_sum - last_count_sum)) / INTERVAL,
               (double)frame_sum / run_time_sum, run_time_sum);
        snprintf(hjk_encoder_buf, sizeof(hjk_encoder_buf) - 1,
                 "thread sum %d, frame %d, curr fps %2.2f, avg fps %2.2f, run "
                 "time sum %ds \n",
                 tinfo[0].thread_count, count_sum,
                 ((double)(count_sum - last_count_sum)) / INTERVAL,
                 (double)frame_sum / run_time_sum, run_time_sum);
        last_count_sum = count_sum;

        printf("\r");
        fflush(stdout);
    }

    printf("thread thread_encode_statics exit\n");
    return NULL;
}

static int init_more_thread(int argc, char *argv[])
{
    struct thread_info *tinfo = NULL;
    int s = 0, i = 0, opt = 0, num_threads = 1;
    void *res = NULL;

    int size = 0, err = 0;
    int width = 0, height = 0;
    enum AVPixelFormat sw_format = AV_PIX_FMT_NONE;
    int read_cycle = 0;

    /* stat thread */
    pthread_t statics_tid = 0;

    num_threads = atoi(argv[2]);
    if (num_threads < 1) {
        fprintf(stderr, "invalid num_threads=%d\n", num_threads);
        return -1;
    }
    /* Create one thread for each command-line argument */
    read_cycle = atoi(argv[3]);
    width = atoi(argv[4]);
    height = atoi(argv[5]);
    if (!strcmp(argv[6], "nv12")) {
        sw_format = AV_PIX_FMT_NV12;
    } else if (!strcmp(argv[6], "yuyv422")) {
        sw_format = AV_PIX_FMT_YUYV422;
    } else {
        fprintf(stderr, "not support sw format");
        return -1;
    }

    tinfo = calloc(num_threads, sizeof(struct thread_info));
    if (tinfo == NULL) {
        fprintf(stderr, "calloc");
        return -1;
    }

    for (i = 0; i < num_threads; i++) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        tinfo[i].thread_count = num_threads;
        tinfo[i].thread_index = i + 1;
        tinfo[i].m_width = width;
        tinfo[i].m_height = height;
        tinfo[i].m_sw_format = sw_format;
        tinfo[i].m_read_cycle = read_cycle;
        tinfo[i].m_read_yuv_num = 0;
        tinfo[i].m_encoder_jpeg_num = 0;
        snprintf(tinfo[i].m_yuv_name, sizeof(tinfo[i].m_yuv_name) - 1, "%s", argv[1]);
        if (argc > 7) {
            snprintf(tinfo[i].m_jpg_name,
                     sizeof(tinfo[i].m_jpg_name) - 1,
                     "%s_t%d_%dx%d.jpg", argv[7], i, width,
                     height);
        } else {
            tinfo[i].m_jpg_name[0] = '\0';
        }

        s = pthread_create(&tinfo[i].thread_id, NULL /*&attr*/,
                           &thread_encoder, &tinfo[i]);
        if (s != 0) {
            fprintf(stderr, "pthread_create");
            free(tinfo);
            return -1;
        }
    }

    s = pthread_create(&statics_tid, NULL, thread_encode_statics, (void *)tinfo);
    if (s != 0) {
        fprintf(stderr, "pthread_create");
        free(tinfo);
        return -1;
    }

    for (i = 0; i < num_threads; i++) {
        s = pthread_join(tinfo[i].thread_id, &res);
        if (s != 0) {
            fprintf(stderr, "pthread_join");
            free(tinfo);
            return -1;
        }

        printf("Joined with thread %d exit\n", tinfo[i].thread_index);
        if (NULL != res) {
            printf("; returned value was %s\n", (char *)res);
            free(res); /* Free memory allocated by thread */
        }
    }

    g_run_status = 2;
    pthread_join(statics_tid, NULL);

    free(tinfo);

    return 0;
}

int main(int argc, char *argv[])
{

    if (argc < 7) {
        fprintf(stderr,
                "Usage: %s <input file> <thread_sum> <cyc_read_file> <width> "
                "<height> <sw_format> [output file]\n",
                argv[0]);

        fprintf(stderr, "sw_format: nv12 yuyv422 \n");

        fprintf(stderr,
                "%s 1920x1080_nv12.yuv 1 0 1920 1080 nv12 outfile \n",
                argv[0]);
        return -1;
    }

    if (init_more_thread(argc, argv) < 0) {
        fprintf(stderr, "call init_more_thread failed!\n");
        return -1;
    }

    return 0;
}
