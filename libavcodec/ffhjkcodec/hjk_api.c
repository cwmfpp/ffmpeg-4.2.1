#include "hjk_api.h" 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
#0  hj_Init (size=0) at libavutil/../libavcodec/hj_api.c:71
#1  0x00007ffffd78beae in hj_device_create (device_ctx=0x84046c0, device=0x0, opts=0x0, flags=0) at libavutil/hwcontext_hjk.c:337
#2  0x00007ffffd78a776 in av_hwdevice_ctx_create (pdevice_ref=0x8203020 <hw_device_ctx>, type=AV_HWDEVICE_TYPE_HJK, device=0x0, opts=0x0, flags=0)
    at libavutil/hwcontext.c:594
#3  0x000000000800155f in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:130
*/
static int hj_Init(int hj_size)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

static int hj_DeviceGetCount(int *hj_nb_devices)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

static int hj_DeviceGet(HJdevice * hj_device, int hj_idx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

static int hj_DeviceGetName(char *dev_name, int dev_name_size, HJdevice hj_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == dev_name || dev_name_size <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }
    snprintf(dev_name, dev_name_size - 1, "Hello HJK Device");

    return HJK_SUCCESS;
}

static int hj_DeviceComputeCapability(int *dev_major, int *dev_minor,
								  HJdevice hj_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

/*
#0  hj_CtxCreate (hj_context_internal=0x8404720, size=0, hj_device=0x7ffffd9eaf60 <ff_hwcontext_type_hjk>) at libavutil/../libavcodec/hj_api.c:74
#1  0x00007ffffd78c809 in hj_device_create (device_ctx=0x84046c0, device=0x0, opts=0x0, flags=0) at libavutil/hwcontext_hjk.c:345
#2  0x00007ffffd78b03f in av_hwdevice_ctx_create (pdevice_ref=0x8203020 <hw_device_ctx>, type=AV_HWDEVICE_TYPE_HJK, device=0x0, opts=0x0, flags=0)
    at libavutil/hwcontext.c:594
#3  0x000000000800155f in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:130
*/
static int hj_CtxCreate(HJcontext * hj_context_internal, int hj_size,
					HJdevice hj_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    *hj_context_internal = (HJcontext)malloc(sizeof(HJcontext));
    if (NULL == *hj_context_internal) {
        return HJK_ENC_ERR_OUT_OF_MEMORY;
    }

    return HJK_SUCCESS;
}

/*
#0  hj_CtxPopCurrent (dummy=0x7ffffffedcb0) at libavutil/../libavcodec/hj_api.c:15
#1  0x00007ffffd78b4a0 in hj_pool_alloc (opaque=0x8406420, size=3110400) at libavutil/hwcontext_hjk.c:117
#2  0x00007ffffd777b96 in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777cf2 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b639 in hj_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a523 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e50 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd78a042 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hj_CtxPopCurrent(HJcontext *hj_dummy)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

/*
#0  hj_CtxPushCurrent (hj_context=0x8404bc0) at libavutil/../libavcodec/hj_api.c:8
#1  0x00007ffffd78b3ac in hj_pool_alloc (opaque=0x8406420, size=3110400) at libavutil/hwcontext_hjk.c:102
#2  0x00007ffffd777b96 in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777cf2 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b639 in hj_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a523 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e50 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd78a042 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hj_CtxPushCurrent(HJcontext hj_context)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

/*
alloc memory for dst AFrame

#0  hj_MemAlloc (data=0x7ffffffedcb8, size=3317760) at libavutil/../libavcodec/hj_api.c:92
#1  0x00007ffffd78b3af in hj_pool_alloc (opaque=0x8406420, size=3317760) at libavutil/hwcontext_hjk.c:106
#2  0x00007ffffd777b4c in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777ca8 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b5ef in hj_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a4d9 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e06 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd789ff8 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hj_MemAlloc(HJdeviceptr *hj_data, int hj_size)
{
    printf("%d %s size=%d\n", __LINE__, __FUNCTION__, hj_size);

    if (hj_size <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    if (NULL == (*hj_data = (HJdeviceptr)malloc(hj_size))) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    return HJK_SUCCESS;
}

/*
encode data
#0  hj_Memcpy2DAsync (hj_cpy=0x7ffffffede90, stream=0x0) at libavutil/../libavcodec/hj_api.c:125
#1  0x00007ffffd78bc0e in hj_transfer_data_to (ctx=0x8406420, dst=0x8408f70, src=0x8409230) at libavutil/hwcontext_hjk.c:266
#2  0x00007ffffd78a334 in av_hwframe_transfer_data (dst=0x8408f70, src=0x8409230, flags=0) at libavutil/hwcontext.c:460
#3  0x0000000008001a47 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:194
*/
/*
decode data
#0  hj_Memcpy2DAsync (hj_cpy=0x7ffffffede90, stream=0x0) at libavutil/../libavcodec/ffhjkcodec/hjk_api.c:152
#1  0x00007ffffd29c85d in hjk_transfer_data_from (ctx=0x84096c0, dst=0x840ad70, src=0x840c4d0) at libavutil/hwcontext_hjk.c:224
#2  0x00007ffffd29b194 in av_hwframe_transfer_data (dst=0x840ad70, src=0x840c4d0, flags=0) at libavutil/hwcontext.c:454
#3  0x00007ffffd29b0aa in transfer_data_alloc (dst=0x840c210, src=0x840c4d0, flags=0) at libavutil/hwcontext.c:429
#4  0x00007ffffd29b149 in av_hwframe_transfer_data (dst=0x840c210, src=0x840c4d0, flags=0) at libavutil/hwcontext.c:449
#5  0x00000000080014be in decode_write (avctx=0x84068b0, packet=0x7ffffffee060) at doc/examples/hjkjpg_hw_decode.c:110
#6  0x0000000008001aee in main (argc=4, argv=0x7ffffffee1a8) at doc/examples/hjkjpg_hw_decode.c:234
*/
static int hj_Memcpy2DAsync(HJK_MEMCPY2D *hj_cpy, HJstream hj_stream)
{
    int i = 0;
    //printf("%d %s height=%d srchost=%p dstdevice=%p\n", __LINE__, __FUNCTION__, hj_cpy->Height, hj_cpy->srcHost, hj_cpy->dstDevice);
    if (hj_cpy->dstPitch < hj_cpy->WidthInBytes ||
        hj_cpy->srcPitch < hj_cpy->WidthInBytes) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    if (HJ_MEMORYTYPE_HOST == hj_cpy->srcMemoryType &&
        HJ_MEMORYTYPE_DEVICE == hj_cpy->dstMemoryType) {
        /* src encoder data */
        printf("%d %s \n", __LINE__, __FUNCTION__);
        for (i = 0; i < hj_cpy->Height; i++) {
            memcpy((uint8_t *)hj_cpy->dstDevice + i * hj_cpy->dstPitch,
                   (uint8_t *)hj_cpy->srcHost + i * hj_cpy->srcPitch,
                   hj_cpy->WidthInBytes);
        }
    } else if (HJ_MEMORYTYPE_DEVICE == hj_cpy->srcMemoryType &&
               HJ_MEMORYTYPE_HOST == hj_cpy->dstMemoryType) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        /* src hj_video_decoder data */
        /* copy decode data to host*/
        /* hj_cpy->srcDevice = hj_devptr; 
        hj_vidMapVideoFrame(HJvideodecoder hj_video_decoder, unsigned int hj_idx,
						  HJdeviceptr *hj_devptr, unsigned int *pitch,*/
        printf("%d %s hj_cpy->Height=%d\n", __LINE__, __FUNCTION__, hj_cpy->Height);
        for (i = 0; i < hj_cpy->Height; i++) {
            memcpy((uint8_t *)hj_cpy->dstHost + i * hj_cpy->dstPitch,
                   (uint8_t *)hj_cpy->srcDevice + i * hj_cpy->srcPitch,
                   hj_cpy->WidthInBytes);
        }


    } else {
        printf("%d %s not support data copy\n", __LINE__, __FUNCTION__);
    }
        return HJK_SUCCESS;
}

static int hj_StreamSynchronize(HJstream hj_stream)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_SUCCESS;
}

/*
#0  hj_MemFree (data=0x7ffff8690010) at libavutil/../libavcodec/hj_api.c:85
#1  0x00007ffffd78b2cc in hj_buffer_free (opaque=0x8406420, data=0x7ffff8690010 "") at libavutil/hwcontext_hjk.c:85
#2  0x00007ffffd777a0d in buffer_pool_free (pool=0x8406520) at libavutil/buffer.c:264
#3  0x00007ffffd777ace in av_buffer_pool_uninit (ppool=0x8406490) at libavutil/buffer.c:285
#4  0x00007ffffd789aa9 in hwframe_ctx_free (opaque=0x0, data=0x8406420 "\200<\237\375\377\177") at libavutil/hwcontext.c:229
#5  0x00007ffffd777473 in buffer_replace (dst=0x8404fd0, src=0x0) at libavutil/buffer.c:120
#6  0x00007ffffd7774c6 in av_buffer_unref (buf=0x8404fd0) at libavutil/buffer.c:130
#7  0x00007ffffe3ad95b in avcodec_close (avctx=0x8404be0) at libavcodec/utils.c:1126
#8  0x00007ffffe21062b in avcodec_free_context (pavctx=0x7ffffffedf48) at libavcodec/options.c:178
#9  0x0000000008001bea in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:220
*/
static int hj_MemFree(HJdeviceptr dev_data)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL != dev_data) {
        free(dev_data);
    }

    return HJK_SUCCESS;
}

static int hj_CtxDestroy(HJcontext hj_context_internal)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_context_internal) {
    printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }

    free(hj_context_internal);

    return HJK_SUCCESS;
}

static const struct {
    HJresult hjerr; /* hj internal error */
    int         ff_averr; /* map ffmpeg error */
    const char *desc;
} hjenc_internal_errors[] = {
    { HJK_SUCCESS,                      0,                "success"                  },
    { HJK_FAILED,         -1/*AVERROR(ENOENT)*/,  "failed"         },
};

static HJresult hj_GetErrorName(HJresult hj_error, const char** hj_pstr)
{
    int i;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    for (i = 0; i < (sizeof(hjenc_internal_errors) / sizeof(hjenc_internal_errors[0])); i++) {
        if (hjenc_internal_errors[i].hjerr == hj_error) {
            if (hj_pstr)
                *hj_pstr = hjenc_internal_errors[i].desc;
            return hjenc_internal_errors[i].ff_averr;
        }
    }
    if (hj_pstr)
        *hj_pstr= "unknown internal error name";

    return HJK_SUCCESS;
}

static HJresult hj_GetErrorString(HJresult hj_error, const char** hj_pstr)
{
    int i;
    printf("%d %s \n", __LINE__, __FUNCTION__);
    for (i = 0; i < (sizeof(hjenc_internal_errors) / sizeof(hjenc_internal_errors[0])); i++) {
        if (hjenc_internal_errors[i].hjerr == hj_error) {
            if (hj_pstr)
                *hj_pstr = hjenc_internal_errors[i].desc;
            return hjenc_internal_errors[i].ff_averr;
        }
    }
    if (hj_pstr)
        *hj_pstr= "unknown internal error string";

    return HJK_SUCCESS;
}

static HjkFunctions hj_functions = {
    .hjInit = hj_Init,
    .hjDeviceGetCount = hj_DeviceGetCount,
    .hjDeviceGet = hj_DeviceGet,
    .hjDeviceGetName = hj_DeviceGetName,
    .hjDeviceComputeCapability = hj_DeviceComputeCapability,
    .hjCtxCreate = hj_CtxCreate,
    .hjCtxPopCurrent = hj_CtxPopCurrent,
    .hjCtxPushCurrent = hj_CtxPushCurrent,
    .hjMemAlloc = hj_MemAlloc,
    .hjMemcpy2DAsync = hj_Memcpy2DAsync,
    .hjStreamSynchronize = hj_StreamSynchronize,
    .hjMemFree = hj_MemFree,
    .hjCtxDestroy = hj_CtxDestroy,
    .hjGetErrorName = hj_GetErrorName,
    .hjGetErrorString = hj_GetErrorString,
};


int hjk_load_functions(HjkFunctions **hjk_dl, void *avctx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    *hjk_dl = &hj_functions;

    return HJK_ENC_SUCCESS;
}

////
#define HJK_ENC_REGI_RES_NUM    6

typedef struct _HjkEncoderInterOut {
    void *m_enc_out_virtual_addr;
    int m_enc_out_size;
    int m_enc_data_len;
} HjkEncoderInterOut;

typedef struct _HjkEncoderInfo {
    HJK_ENC_INITIALIZE_PARAMS m_enc_init_params;
    HJK_ENC_REGISTER_RESOURCE m_list_enc_reg_res[HJK_ENC_REGI_RES_NUM];
} HjkEncoderInfo;

/* HJK_ENCODE_API_FUNCTION_LIST start */
/* p1 open hw, open encoder*/
static int hjk_EncOpenEncodeSessionEx(
	HJK_ENC_OPEN_ENCODE_SESSION_EX_PARAMS *open_params, void **hj_enc_handle)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);
    p_hjk_enc_info = (HjkEncoderInfo *)malloc(sizeof(HjkEncoderInfo));
    if (NULL == p_hjk_enc_info) {
        return HJK_ENC_ERR_OUT_OF_MEMORY;
    }

    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        p_hjk_enc_info->m_list_enc_reg_res[i].resourceToRegister = NULL;
        p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource = NULL;
    }
    *hj_enc_handle = (void *)p_hjk_enc_info;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodeGUIDCount(void *hj_enc_handle, int *count)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == count) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    *count = 1;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodeGUIDs(void *hj_enc_handle, void *guid, int count,
							int *ptr_count)
{
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (count <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    for (i = 0; i < count; i++) {
        *((int *)guid + i) = HJK_ENC_CODEC_MJPEG_GUID;
    }
    *ptr_count = i;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodeCaps(void *hj_enc_handle, int encodeGUID,
						   HJK_ENC_CAPS_PARAM *caps_params, int *val)
{
    int ret = HJK_ENC_SUCCESS;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    switch (caps_params->capsToQuery) {
    case HJK_ENC_CAPS_SUPPORT_YUV444_ENCODE:break;
    case HJK_ENC_CAPS_SUPPORT_LOSSLESS_ENCODE:break;
    case HJK_ENC_CAPS_WIDTH_MAX:
        *val = 4096;
        break;
    case HJK_ENC_CAPS_HEIGHT_MAX:
        *val = 4096;
        break;
    case HJK_ENC_CAPS_NUM_MAX_BFRAMES:
        break;
    case HJK_ENC_CAPS_SUPPORT_FIELD_ENCODING:
        break;
    case HJK_ENC_CAPS_SUPPORT_10BIT_ENCODE:
        break;
    case HJK_ENC_CAPS_SUPPORT_LOOKAHEAD:
        break;
    case HJK_ENC_CAPS_SUPPORT_TEMPORAL_AQ:
        break;
    case HJK_ENC_CAPS_SUPPORT_WEIGHTED_PREDICTION:
        break;
    case HJK_ENC_CAPS_SUPPORT_CABAC:
        *val = 1;
        break;
    case HJK_ENC_CAPS_SUPPORT_BFRAME_REF_MODE:break;
    case HJK_ENC_CAPS_SUPPORT_MULTIPLE_REF_FRAMES:break;
    case HJK_ENC_CAPS_SUPPORT_DYN_BITRATE_CHANGE:break;
    }

    return ret;
}

static int hjk_EncGetEncodePresetConfigEx(void *hj_enc_handle, int encodeGUID,
									 int presetGUID, int tuningInfo,
									 HJK_ENC_PRESET_CONFIG *preset_config)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodePresetConfig(void *hj_enc_handle,
								   int encodeGUID,
								   int presetGUID,
								   HJK_ENC_PRESET_CONFIG *preset_config)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncInitializeEncoder(void *hj_enc_handle, HJK_ENC_INITIALIZE_PARAMS *init_encode_params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == init_encode_params) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;

    memcpy(&p_hjk_enc_info->m_enc_init_params, init_encode_params, sizeof(*init_encode_params));

    return HJK_ENC_SUCCESS;
}

static int hjk_EncSetIOHjkStreams(void *hj_enc_handle, HJstream *hj_stream,
							  HJstream *hj_stream1)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncCreateInputBuffer(void *hj_enc_handle, HJK_ENC_CREATE_INPUT_BUFFER *allocSurf)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_EncCreateBitstreamBuffer (hj_enc_handle=0x8406500, allocOut=0x7ffffffedb90) at libavutil/../libavcodec/hjk_api.c:386
#1  0x00007ffffe03f014 in hjkenc_alloc_surface (avctx=0x8404be0, hj_idx=0) at libavcodec/hjkenc.c:1308
#2  0x00007ffffe03f303 in hjkenc_setup_surfaces (avctx=0x8404be0) at libavcodec/hjkenc.c:1354
#3  0x00007ffffe03fb73 in ff_hjkenc_encode_init (avctx=0x8404be0) at libavcodec/hjkenc.c:1507
#4  0x00007ffffe3acfb5 in avcodec_open2 (avctx=0x8404be0, codec=0x7ffffec097c0 <ff_mjpeg_hjkenc_encoder>, options=0x0) at libavcodec/utils.c:935
#5  0x0000000008001777 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:161
*/
static int hjk_EncCreateBitstreamBuffer(void *hj_enc_handle, HJK_ENC_CREATE_BITSTREAM_BUFFER *allocOut)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    int out_size = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == allocOut) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;

    out_size = p_hjk_enc_info->m_enc_init_params.encodeWidth *
               p_hjk_enc_info->m_enc_init_params.encodeHeight;

    if (out_size <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    switch (p_hjk_enc_info->m_enc_init_params.encodeConfig->profileGUID)
    {
    case HJK_ENC_MJPEG_PROFILE_BASELINE_GUID:
        out_size = out_size * 2;
        break;
    case HJK_ENC_MJPEG_PROFILE_MAIN_GUID:
        out_size = out_size * 3;
        break;
    case HJK_ENC_MJPEG_PROFILE_HIGH_GUID:
        out_size = out_size * 3;
        break;
    case HJK_ENC_MJPEG_PROFILE_HIGH_444_GUID:
        out_size = out_size * 3;
        break;
    
    default:
        break;
    }

    if (NULL == (p_hjk_enc_inter_out = (HjkEncoderInterOut *)malloc(
                     sizeof(*p_hjk_enc_inter_out)))) {
        return HJK_ENC_ERR_OUT_OF_MEMORY;
    }
    if (NULL ==
        (p_hjk_enc_inter_out->m_enc_out_virtual_addr = malloc(out_size))) {
        free(p_hjk_enc_inter_out);
        return HJK_ENC_ERR_OUT_OF_MEMORY;
    }
    p_hjk_enc_inter_out->m_enc_out_size = out_size;
    p_hjk_enc_inter_out->m_enc_data_len = 0;
    allocOut->bitstreamBuffer = p_hjk_enc_inter_out;
    allocOut->size = sizeof(*p_hjk_enc_inter_out);
    printf("%d %s allocOut->bitstreamBuffer=%p m_enc_out_virtual_addr=%p\n",
           __LINE__, __FUNCTION__, allocOut->bitstreamBuffer,
           p_hjk_enc_inter_out->m_enc_out_virtual_addr);

    return HJK_ENC_SUCCESS;
}

static int
hjk_EncGetSequenceParams(void *hj_enc_handle,
                         HJK_ENC_SEQUENCE_PARAM_PAYLOAD *hj_payload) {
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncRegisterResource(void *hj_enc_handle, HJK_ENC_REGISTER_RESOURCE *hj_reg)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == hj_reg) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (NULL == p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource) {
            // find ok, register i resource
            hj_reg->registeredResource =
                &p_hjk_enc_info->m_list_enc_reg_res[i]; // todo
            memcpy(&p_hjk_enc_info->m_list_enc_reg_res[i], hj_reg, sizeof(*hj_reg));
            break;
        }
    }
    return HJK_ENC_SUCCESS;
}

static int hjk_EncReconfigureEncoder(void *hj_enc_handle, HJK_ENC_RECONFIGURE_PARAMS *hj_params)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncMapInputResource(void *hj_enc_handle,
							  HJK_ENC_MAP_INPUT_RESOURCE *hj_in_map)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == hj_in_map) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource ==
            hj_in_map->registeredResource) {
            // find ok, map
            printf("%d %s map find ok\n", __LINE__, __FUNCTION__);
            hj_in_map->mappedResource =
                &p_hjk_enc_info->m_list_enc_reg_res[i]; /*.resourceToRegister;*/
            break;
        }
    }
    return HJK_ENC_SUCCESS;
}

static int hjk_EncLockInputBuffer(void *hj_enc_handle, HJK_ENC_LOCK_INPUT_BUFFER *lockBufferParams)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnlockInputBuffer(void *hj_enc_handle, HJK_ENC_INPUT_PTR hjk_input_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/* p2 enc encoder 
#0  hjk_EncEncodePicture (hj_enc_handle=0x8406500, hj_params=0x7ffffffedcd0) at libavutil/../libavcodec/hjk_api.c:409
#1  0x00007ffffe0417e3 in ff_hjkenc_send_frame (avctx=0x8404be0, frame=0x8408f70) at libavcodec/hjkenc.c:2139
#2  0x00007ffffdd53433 in avcodec_send_frame (avctx=0x8404be0, frame=0x8408f70) at libavcodec/encode.c:403
#3  0x00000000080012ae in encode_write (avctx=0x8404be0, frame=0x8408f70, fout=0x8404490) at doc/examples/hjkjpg_encode.c:83
#4  0x0000000008001af3 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:200
*/
static int hjk_EncEncodePicture(void *hj_enc_handle, HJK_ENC_PIC_PARAMS *hj_params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HJK_ENC_REGISTER_RESOURCE *p_enc_reg_res = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    if (NULL == hj_enc_handle || NULL == hj_params) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    printf("%d %s \n", __LINE__, __FUNCTION__);
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_enc_reg_res = (HJK_ENC_REGISTER_RESOURCE *)hj_params->inputBuffer;
    if (NULL == p_enc_reg_res) {
        return HJK_ENC_SUCCESS;
    }
    printf("%d %s in buf p_enc_reg_res->resourceToRegister=%p\n", __LINE__, __FUNCTION__, p_enc_reg_res->resourceToRegister);
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)hj_params->outputBitstream; 
    printf("%d %s out buf hj_params->outputBitstream=%p\n", __LINE__, __FUNCTION__,
           hj_params->outputBitstream);

#if 1
    //test copy raw yuv data
    int encoder_data_len = 0;

    encoder_data_len = p_enc_reg_res->pitch * p_enc_reg_res->height;
    switch (p_enc_reg_res->bufferFormat) {
    case HJK_ENC_BUFFER_FORMAT_YV12_PL:
        encoder_data_len = (encoder_data_len * 3) / 2;
        break;

    case HJK_ENC_BUFFER_FORMAT_NV12_PL:
        encoder_data_len = (encoder_data_len * 3) / 2;
        break;

    case HJK_ENC_BUFFER_FORMAT_YUV420_10BIT:
        encoder_data_len = (encoder_data_len * 3) / 2;
        break;

    case HJK_ENC_BUFFER_FORMAT_YUV444_PL:
        encoder_data_len = encoder_data_len * 3;
        break;

    case HJK_ENC_BUFFER_FORMAT_YUV444_10BIT:
        encoder_data_len = encoder_data_len * 3;
        break;

    case HJK_ENC_BUFFER_FORMAT_ARGB:
        encoder_data_len = encoder_data_len * 4;
        break;

    case HJK_ENC_BUFFER_FORMAT_ABGR:
        encoder_data_len = encoder_data_len * 4;
        break;

    case HJK_ENC_BUFFER_FORMAT_UNDEFINED:
        encoder_data_len = 0;
        break;

    default:
        encoder_data_len = 0;
        break;
    }

    if (0 == encoder_data_len) {
        return HJK_ENC_ERR_UNSUPPORTED_PARAM;
    }
    memcpy(p_hjk_enc_inter_out->m_enc_out_virtual_addr,
           p_enc_reg_res->resourceToRegister,
           encoder_data_len);
    p_hjk_enc_inter_out->m_enc_data_len = encoder_data_len;
#endif

    return HJK_ENC_SUCCESS;
}

/* p3 start encode data*/
static int hjk_EncLockBitstream(void *hj_enc_handle, HJK_ENC_LOCK_BITSTREAM *lock_params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == lock_params) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    if (NULL == lock_params->outputBitstream) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)lock_params->outputBitstream;
    printf("%d %s outputBitstream=%p  m_enc_out_virtual_addr=%p\n", __LINE__, __FUNCTION__, lock_params->outputBitstream, p_hjk_enc_inter_out->m_enc_out_virtual_addr);
    lock_params->bitstreamBufferPtr = p_hjk_enc_inter_out->m_enc_out_virtual_addr;
    lock_params->bitstreamSizeInBytes = p_hjk_enc_inter_out->m_enc_data_len;
    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnlockBitstream(void *hj_enc_handle, HJK_ENC_OUTPUT_PTR hjk_output_surface)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == hjk_output_surface) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)hjk_output_surface;
    p_hjk_enc_inter_out->m_enc_data_len = 0;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnmapInputResource(
	void *hj_enc_handle, HJK_ENC_INPUT_PTR mappedResource)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == mappedResource) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (&p_hjk_enc_info->m_list_enc_reg_res[i]/*.resourceToRegister*/ ==
            mappedResource) {
            // find ok, map
            // todo
            printf("%d %s unmap find ok\n", __LINE__, __FUNCTION__);
            break;
        }
    }
    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnregisterResource(void *hj_enc_handle,
								HJK_ENC_REGISTERED_PTR hjk_regptr)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle || NULL == hjk_regptr) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource ==
            hjk_regptr) {
            // find ok, release i resource
            printf("%d %s find ok, release i resource\n", __LINE__,
                   __FUNCTION__);
            p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource = NULL;
            break;
        }
    }

    return HJK_ENC_SUCCESS;
}
static int hjk_EncDestroyBitstreamBuffer(void *hj_enc_handle,
									HJK_ENC_OUTPUT_PTR hjk_output_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL != hjk_output_surface) {
        HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
        p_hjk_enc_inter_out = hjk_output_surface;
        if (NULL != p_hjk_enc_inter_out->m_enc_out_virtual_addr) {
            free(p_hjk_enc_inter_out->m_enc_out_virtual_addr);
            p_hjk_enc_inter_out->m_enc_out_virtual_addr = NULL;
            p_hjk_enc_inter_out->m_enc_out_size = 0;
            p_hjk_enc_inter_out->m_enc_data_len = 0;
        }
        free(p_hjk_enc_inter_out);
    }

    return HJK_ENC_SUCCESS;
}

static int hjk_EncDestroyEncoder(void *hj_enc_handle)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hj_enc_handle) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)hj_enc_handle;

    free(p_hjk_enc_info);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncDestroyInputBuffer(void *hj_enc_handle,
								HJK_ENC_INPUT_PTR hjk_input_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetLastErrorString(void *hj_enc_handle)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}
/* HJK_ENCODE_API_FUNCTION_LIST end */

static HJK_ENCODE_API_FUNCTION_LIST hjk_encode_api_function_list_member = {
    .hjkEncOpenEncodeSessionEx = hjk_EncOpenEncodeSessionEx,
    .hjkEncGetEncodeGUIDCount = hjk_EncGetEncodeGUIDCount,
    .hjkEncGetEncodeGUIDs = hjk_EncGetEncodeGUIDs,
    .hjkEncGetEncodeCaps = hjk_EncGetEncodeCaps,
    .hjkEncGetEncodePresetConfigEx = hjk_EncGetEncodePresetConfigEx,
    .hjkEncGetEncodePresetConfig = hjk_EncGetEncodePresetConfig,
    .hjkEncInitializeEncoder = hjk_EncInitializeEncoder,
    .hjkEncSetIOHjkStreams = hjk_EncSetIOHjkStreams,
    .hjkEncCreateInputBuffer = hjk_EncCreateInputBuffer,
    .hjkEncCreateBitstreamBuffer = hjk_EncCreateBitstreamBuffer,
    .hjkEncGetSequenceParams = hjk_EncGetSequenceParams,
    .hjkEncRegisterResource = hjk_EncRegisterResource,
    .hjkEncReconfigureEncoder = hjk_EncReconfigureEncoder,
    .hjkEncMapInputResource = hjk_EncMapInputResource,
    .hjkEncLockInputBuffer = hjk_EncLockInputBuffer,
    .hjkEncUnlockInputBuffer = hjk_EncUnlockInputBuffer,
    .hjkEncEncodePicture = hjk_EncEncodePicture,
    .hjkEncLockBitstream = hjk_EncLockBitstream,
    .hjkEncUnlockBitstream = hjk_EncUnlockBitstream,
    .hjkEncUnmapInputResource = hjk_EncUnmapInputResource,
    .hjkEncUnregisterResource = hjk_EncUnregisterResource,
    .hjkEncDestroyBitstreamBuffer = hjk_EncDestroyBitstreamBuffer,
    .hjkEncDestroyEncoder = hjk_EncDestroyEncoder,
    .hjkEncDestroyInputBuffer = hjk_EncDestroyInputBuffer,
    .hjkEncGetLastErrorString = hjk_EncGetLastErrorString,
};

/* HjkencFunctions start*/
static int hjk_EncodeAPIGetMaxSupportedVersion(uint32_t *hjkenc_max_ver)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hjkenc_max_ver) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    *hjkenc_max_ver = 1 << 4 | 1;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncodeAPICreateInstance(HJK_ENCODE_API_FUNCTION_LIST *hjkenc_funcs)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    memcpy(hjkenc_funcs, &hjk_encode_api_function_list_member,
           sizeof(hjk_encode_api_function_list_member));

    return HJK_ENC_SUCCESS;
}
/* HjkencFunctions end*/

static HjkencFunctions hjk_enc_functions = {
    .HjkEncodeAPIGetMaxSupportedVersion = hjk_EncodeAPIGetMaxSupportedVersion,
    .HjkEncodeAPICreateInstance = hjk_EncodeAPICreateInstance,
};

int hjkenc_load_functions(HjkencFunctions **hjkenc_dl, void *avctx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    *hjkenc_dl = &hjk_enc_functions;

    return HJK_ENC_SUCCESS;
}


void hjkenc_free_functions(HjkencFunctions **hjkenc_dl)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hjkenc_dl) {
        return;
    }
    *hjkenc_dl = NULL;

    return;
}

void hjk_free_functions(HjkFunctions **hjk_dl)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hjk_dl) {
        return;
    }
    *hjk_dl = NULL;

    return;
}


/*=====================================================================*/
/*hjk dec start*/

typedef struct _HjkDecoderBuffer {
    void *m_dec_output;
    int m_dec_output_len;
    int m_pitch;
}HjkDecoderBuffer;

typedef struct _HjkDecoderInfo {
    int a;
    HjkDecoderBuffer *m_decoce_data_output;
    HJVIDDECODECREATEINFO m_hj_vid_info;
} HjkDecoderInfo;

static int hj_vidGetDecoderCaps(HJVIDDECODECAPS *caps)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == caps) {
        return -1;
    }

    caps->bIsSupported = 1;
    caps->nMinWidth = 2;
    caps->nMaxWidth = 4096;
    caps->nMinHeight = 2;
    caps->nMaxHeight = 4096;
    caps->nMaxMBCount = 100000;

    return HJK_SUCCESS;
}

static int hj_vidCreateDecoder(HJvideodecoder *hj_video_decoder,
						  HJVIDDECODECREATEINFO *hj_params)
{
    HjkDecoderInfo *p_hjk_decoder_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_video_decoder || NULL == hj_params) {
        return -1;
    }
    if (NULL == (p_hjk_decoder_info = (HjkDecoderInfo *)malloc(sizeof(*p_hjk_decoder_info)))) {
        return HJK_ENOMEM;
    }
    if (NULL == (p_hjk_decoder_info->m_decoce_data_output =
                     malloc(hj_params->ulNumOutputSurfaces * sizeof(HjkDecoderBuffer)))) {
        free(p_hjk_decoder_info);
        return HJK_ENOMEM;
    }

    memcpy(&p_hjk_decoder_info->m_hj_vid_info, hj_params, sizeof(*hj_params));
    *hj_video_decoder = p_hjk_decoder_info;

    return HJK_SUCCESS;
}

static int hj_vidDecodePicture(HJvideodecoder hj_video_decoder,
						  HJVIDPICPARAMS *hj_pic_params)
{
    HjkDecoderInfo *p_hjk_decoder_info = NULL;
    HjkDecoderBuffer *p_hjk_dec_buffer = NULL;
    int yuv_size = 0;
    int i = 0;
    uint8_t *decode_data_output = NULL;

    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_video_decoder || NULL == hj_pic_params) {
        printf("%d %s hj_video_decoder=%p hj_pic_params=%p\n", __LINE__, __FUNCTION__,
               hj_video_decoder, hj_pic_params);
        return -1;
    }

    p_hjk_decoder_info = (HjkDecoderInfo *)hj_video_decoder;

    if (hj_pic_params->nBitstreamDataLen <= 0) {
        return HJK_EINVALID;
    }
    yuv_size = (p_hjk_decoder_info->m_hj_vid_info.ulTargetWidth *
                p_hjk_decoder_info->m_hj_vid_info.ulTargetHeight * 3) /
               2;
    decode_data_output = (uint8_t *)malloc(yuv_size);
    if (NULL == decode_data_output) {
        return HJK_ENOMEM;
    }

    for (i = 0; i < yuv_size; i++) {
        decode_data_output[i] = i % 256;
    }
    p_hjk_dec_buffer =
        &p_hjk_decoder_info->m_decoce_data_output[hj_pic_params->CurrPicIdx];
    p_hjk_dec_buffer->m_dec_output = (void *)decode_data_output;
    p_hjk_dec_buffer->m_dec_output_len = yuv_size;
    p_hjk_dec_buffer->m_pitch = p_hjk_decoder_info->m_hj_vid_info.ulTargetWidth;

    return HJK_SUCCESS;
}

static int hj_vidMapVideoFrame(HJvideodecoder hj_video_decoder, unsigned int hj_idx,
						  HJdeviceptr *hj_devptr, unsigned int *hj_pitch,
						  HJVIDPROCPARAMS *vpp)
{
    HjkDecoderInfo *p_hjk_decoder_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_video_decoder || NULL == hj_devptr|| NULL == vpp) {
        return -1;
    }
    p_hjk_decoder_info = (HjkDecoderInfo *)hj_video_decoder;
    *hj_devptr = (HJdeviceptr)(p_hjk_decoder_info->m_decoce_data_output[hj_idx]
                                .m_dec_output);
    *hj_pitch = p_hjk_decoder_info->m_decoce_data_output[hj_idx].m_pitch;

    return HJK_SUCCESS;
}

static int hj_vidUnmapVideoFrame(HJvideodecoder hj_video_decoder, HJdeviceptr hj_devptr)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_video_decoder || NULL == hj_devptr) {
        return -1;
    }

    free(hj_devptr);

    return HJK_SUCCESS;
}

static int hj_vidDestroyDecoder(HJvideodecoder hj_video_decoder)
{
    HjkDecoderInfo *p_hjk_decoder_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_video_decoder) {
        return -1;
    }
    p_hjk_decoder_info = (HjkDecoderInfo *)hj_video_decoder;
    free(p_hjk_decoder_info->m_decoce_data_output);
    free(p_hjk_decoder_info);

    return HJK_SUCCESS;
}


static HjvidFunctions hj_vid_functions = {
    .hjvidGetDecoderCaps = hj_vidGetDecoderCaps,
    .hjvidCreateDecoder = hj_vidCreateDecoder,
    .hjvidDecodePicture = hj_vidDecodePicture,
    .hjvidMapVideoFrame = hj_vidMapVideoFrame,
    .hjvidUnmapVideoFrame = hj_vidUnmapVideoFrame,
    .hjvidDestroyDecoder = hj_vidDestroyDecoder,
};

int hjvid_load_functions(HjvidFunctions **hj_vid_f, void *logctx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_vid_f) {
        return -1;
    }

    *hj_vid_f = &hj_vid_functions;

    return HJK_SUCCESS;
}

void hjvid_free_functions(HjvidFunctions **hj_vid_f)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);
    if (NULL == hj_vid_f) {
        return;
    }

    if (NULL != *hj_vid_f) {
        *hj_vid_f = NULL;
    }

    return;
}
/*hjk dec end*/
