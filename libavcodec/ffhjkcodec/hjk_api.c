#include "hjk_api.h" 

#include <stdio.h>
#include <string.h>

/*
#0  hjk_CtxPushCurrent (hjk_context=0x8404bc0) at libavutil/../libavcodec/hjk_api.c:8
#1  0x00007ffffd78b3ac in hjk_pool_alloc (opaque=0x8406420, size=3110400) at libavutil/hwcontext_hjk.c:102
#2  0x00007ffffd777b96 in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777cf2 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b639 in hjk_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a523 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e50 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd78a042 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hjk_CtxPushCurrent(HJcontext hjk_context)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_CtxPopCurrent (dummy=0x7ffffffedcb0) at libavutil/../libavcodec/hjk_api.c:15
#1  0x00007ffffd78b4a0 in hjk_pool_alloc (opaque=0x8406420, size=3110400) at libavutil/hwcontext_hjk.c:117
#2  0x00007ffffd777b96 in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777cf2 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b639 in hjk_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a523 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e50 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd78a042 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hjk_CtxPopCurrent(HJcontext *dummy)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_DeviceGet(HJdevice * cu_device, int idx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_DeviceGetName(char *name, int name_size, HJdevice cu_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == name || name_size <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }
    snprintf(name, name_size - 1, "Hello HJK Device");

    return HJK_ENC_SUCCESS;
}

static int hjk_DeviceComputeCapability(int *major, int *minor,
								  HJdevice cu_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_CtxCreate (hjk_context_internal=0x8404720, size=0, cu_device=0x7ffffd9eaf60 <ff_hwcontext_type_hjk>) at libavutil/../libavcodec/hjk_api.c:74
#1  0x00007ffffd78c809 in hjk_device_create (device_ctx=0x84046c0, device=0x0, opts=0x0, flags=0) at libavutil/hwcontext_hjk.c:345
#2  0x00007ffffd78b03f in av_hwdevice_ctx_create (pdevice_ref=0x8203020 <hw_device_ctx>, type=AV_HWDEVICE_TYPE_HJK, device=0x0, opts=0x0, flags=0)
    at libavutil/hwcontext.c:594
#3  0x000000000800155f in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:130
*/
static int hjk_CtxCreate(HJcontext * hjk_context_internal, int size,
					HJdevice cu_device)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    *hjk_context_internal = (HJcontext)malloc(sizeof(HJcontext));
    if (NULL == *hjk_context_internal) {
        return HJK_ENC_ERR_OUT_OF_MEMORY;
    }

    return HJK_ENC_SUCCESS;
}

static int hjk_CtxDestroy(HJcontext hjk_context_internal)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == hjk_context_internal) {
    printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }

    free(hjk_context_internal);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_Init (size=0) at libavutil/../libavcodec/hjk_api.c:71
#1  0x00007ffffd78beae in hjk_device_create (device_ctx=0x84046c0, device=0x0, opts=0x0, flags=0) at libavutil/hwcontext_hjk.c:337
#2  0x00007ffffd78a776 in av_hwdevice_ctx_create (pdevice_ref=0x8203020 <hw_device_ctx>, type=AV_HWDEVICE_TYPE_HJK, device=0x0, opts=0x0, flags=0)
    at libavutil/hwcontext.c:594
#3  0x000000000800155f in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:130
*/
static int hjk_Init(int size)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_DeviceGetCount(int *nb_devices)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_MemFree (data=0x7ffff8690010) at libavutil/../libavcodec/hjk_api.c:85
#1  0x00007ffffd78b2cc in hjk_buffer_free (opaque=0x8406420, data=0x7ffff8690010 "") at libavutil/hwcontext_hjk.c:85
#2  0x00007ffffd777a0d in buffer_pool_free (pool=0x8406520) at libavutil/buffer.c:264
#3  0x00007ffffd777ace in av_buffer_pool_uninit (ppool=0x8406490) at libavutil/buffer.c:285
#4  0x00007ffffd789aa9 in hwframe_ctx_free (opaque=0x0, data=0x8406420 "\200<\237\375\377\177") at libavutil/hwcontext.c:229
#5  0x00007ffffd777473 in buffer_replace (dst=0x8404fd0, src=0x0) at libavutil/buffer.c:120
#6  0x00007ffffd7774c6 in av_buffer_unref (buf=0x8404fd0) at libavutil/buffer.c:130
#7  0x00007ffffe3ad95b in avcodec_close (avctx=0x8404be0) at libavcodec/utils.c:1126
#8  0x00007ffffe21062b in avcodec_free_context (pavctx=0x7ffffffedf48) at libavcodec/options.c:178
#9  0x0000000008001bea in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:220
*/
static int hjk_MemFree(HJdeviceptr data)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL != data) {
        free(data);
    }

    return HJK_ENC_SUCCESS;
}

/*
alloc memory for dst AFrame

#0  hjk_MemAlloc (data=0x7ffffffedcb8, size=3317760) at libavutil/../libavcodec/hjk_api.c:92
#1  0x00007ffffd78b3af in hjk_pool_alloc (opaque=0x8406420, size=3317760) at libavutil/hwcontext_hjk.c:106
#2  0x00007ffffd777b4c in pool_alloc_buffer (pool=0x8406520) at libavutil/buffer.c:312
#3  0x00007ffffd777ca8 in av_buffer_pool_get (pool=0x8406520) at libavutil/buffer.c:349
#4  0x00007ffffd78b5ef in hjk_get_buffer (ctx=0x8406420, frame=0x8406630) at libavutil/hwcontext_hjk.c:155
#5  0x00007ffffd78a4d9 in av_hwframe_get_buffer (hwframe_ref=0x8406500, frame=0x8406630, flags=0) at libavutil/hwcontext.c:521
#6  0x00007ffffd789e06 in hwframe_pool_prealloc (ref=0x8406500) at libavutil/hwcontext.c:320
#7  0x00007ffffd789ff8 in av_hwframe_ctx_init (ref=0x8406500) at libavutil/hwcontext.c:373
#8  0x000000000800114f in set_hwframe_ctx (ctx=0x8404be0, hw_device_ctx=0x8404770) at doc/examples/hjkjpg_encode.c:60
#9  0x0000000008001722 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:156
*/
static int hjk_MemAlloc(HJdeviceptr *data, int size)
{
    printf("%d %s size=%d\n", __LINE__, __FUNCTION__, size);

    if (size <= 0) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    if (NULL == (*data = (HJdeviceptr)malloc(size))) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_Memcpy2DAsync (cpy=0x7ffffffede90, stream=0x0) at libavutil/../libavcodec/hjk_api.c:125
#1  0x00007ffffd78bc0e in hjk_transfer_data_to (ctx=0x8406420, dst=0x8408f70, src=0x8409230) at libavutil/hwcontext_hjk.c:266
#2  0x00007ffffd78a334 in av_hwframe_transfer_data (dst=0x8408f70, src=0x8409230, flags=0) at libavutil/hwcontext.c:460
#3  0x0000000008001a47 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:194
*/
static int hjk_Memcpy2DAsync(HJK_MEMCPY2D *cpy, HJstream stream)
{
    int i = 0;

    //printf("%d %s height=%d srchost=%p dstdevice=%p\n", __LINE__, __FUNCTION__, cpy->Height, cpy->srcHost, cpy->dstDevice);
    if (cpy->dstPitch < cpy->WidthInBytes ||
        cpy->srcPitch < cpy->WidthInBytes) {
        return HJK_ENC_ERR_INVALID_PARAM;
    }

    for (i = 0; i < cpy->Height; i++) {
        memcpy((uint8_t *)cpy->dstDevice + i * cpy->dstPitch,
               (uint8_t *)cpy->srcHost + i * cpy->srcPitch, cpy->WidthInBytes);
    }

    return HJK_ENC_SUCCESS;
}

static int hjk_StreamSynchronize(HJstream stream)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_check_GetErrorName(HJKresult error, const char** pstr)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static HjkFunctions hjk_functions = {
    .hjCtxPushCurrent = hjk_CtxPushCurrent,
    .hjCtxPopCurrent = hjk_CtxPopCurrent,
    .hjDeviceGet = hjk_DeviceGet,
    .hjDeviceGetName = hjk_DeviceGetName,
    .hjDeviceComputeCapability = hjk_DeviceComputeCapability,
    .hjCtxCreate = hjk_CtxCreate,
    .hjCtxDestroy = hjk_CtxDestroy,
    .hjInit = hjk_Init,
    .hjDeviceGetCount = hjk_DeviceGetCount,
    .hjMemFree = hjk_MemFree,
    .hjMemAlloc = hjk_MemAlloc,
    .hjMemcpy2DAsync = hjk_Memcpy2DAsync,
    .hjStreamSynchronize = hjk_StreamSynchronize,
    .hjkGetErrorName = hjk_check_GetErrorName,
};


int hjk_load_functions(HjkFunctions **hjk_dl, void *avctx)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    *hjk_dl = &hjk_functions;

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
	HJK_ENC_OPEN_ENCODE_SESSION_EX_PARAMS *open_params, void **handle)
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
    *handle = (void *)p_hjk_enc_info;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodeGUIDCount(void *handle, int *count)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == count) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    *count = 1;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodeGUIDs(void *handle, void *guid, int count,
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

static int hjk_EncGetEncodeCaps(void *handle, int encodeGUID,
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

static int hjk_EncGetEncodePresetConfigEx(void *handle, int encodeGUID,
									 int presetGUID, int tuningInfo,
									 HJK_ENC_PRESET_CONFIG *preset_config)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetEncodePresetConfig(void *handle,
								   int encodeGUID,
								   int presetGUID,
								   HJK_ENC_PRESET_CONFIG *preset_config)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncInitializeEncoder(void *handle, HJK_ENC_INITIALIZE_PARAMS *init_encode_params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == init_encode_params) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)handle;

    memcpy(&p_hjk_enc_info->m_enc_init_params, init_encode_params, sizeof(*init_encode_params));

    return HJK_ENC_SUCCESS;
}

static int hjk_EncSetIOCudaStreams(void *handle, HJstream *cu_stream,
							  HJstream *cu_stream1)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncCreateInputBuffer(void *handle, HJK_ENC_CREATE_INPUT_BUFFER *allocSurf)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/*
#0  hjk_EncCreateBitstreamBuffer (handle=0x8406500, allocOut=0x7ffffffedb90) at libavutil/../libavcodec/hjk_api.c:386
#1  0x00007ffffe03f014 in hjkenc_alloc_surface (avctx=0x8404be0, idx=0) at libavcodec/hjkenc.c:1308
#2  0x00007ffffe03f303 in hjkenc_setup_surfaces (avctx=0x8404be0) at libavcodec/hjkenc.c:1354
#3  0x00007ffffe03fb73 in ff_hjkenc_encode_init (avctx=0x8404be0) at libavcodec/hjkenc.c:1507
#4  0x00007ffffe3acfb5 in avcodec_open2 (avctx=0x8404be0, codec=0x7ffffec097c0 <ff_mjpeg_hjkenc_encoder>, options=0x0) at libavcodec/utils.c:935
#5  0x0000000008001777 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:161
*/
static int hjk_EncCreateBitstreamBuffer(void *handle, HJK_ENC_CREATE_BITSTREAM_BUFFER *allocOut)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    int out_size = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == allocOut) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)handle;

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
    printf("%d %s allocOut->bitstreamBuffer=%p m_enc_out_virtual_addr=%p\n", __LINE__, __FUNCTION__, allocOut->bitstreamBuffer, p_hjk_enc_inter_out->m_enc_out_virtual_addr);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetSequenceParams(void *handle, HJK_ENC_SEQUENCE_PARAM_PAYLOAD *payload)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncRegisterResource(void *handle, HJK_ENC_REGISTER_RESOURCE *reg)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == reg) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (NULL == p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource) {
            // find ok, register i resource
            reg->registeredResource =
                &p_hjk_enc_info->m_list_enc_reg_res[i]; // todo
            memcpy(&p_hjk_enc_info->m_list_enc_reg_res[i], reg, sizeof(*reg));
            break;
        }
    }
    return HJK_ENC_SUCCESS;
}

static int hjk_EncReconfigureEncoder(void *handle, HJK_ENC_RECONFIGURE_PARAMS *params)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncMapInputResource(void *handle,
							  HJK_ENC_MAP_INPUT_RESOURCE *in_map)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == in_map) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource ==
            in_map->registeredResource) {
            // find ok, map
            printf("%d %s map find ok\n", __LINE__, __FUNCTION__);
            in_map->mappedResource =
                &p_hjk_enc_info->m_list_enc_reg_res[i]; /*.resourceToRegister;*/
            break;
        }
    }
    return HJK_ENC_SUCCESS;
}

static int hjk_EncLockInputBuffer(void *handle, HJK_ENC_LOCK_INPUT_BUFFER *lockBufferParams)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnlockInputBuffer(void *handle, HJK_ENC_INPUT_PTR input_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

/* p2 enc encoder 
#0  hjk_EncEncodePicture (handle=0x8406500, params=0x7ffffffedcd0) at libavutil/../libavcodec/hjk_api.c:409
#1  0x00007ffffe0417e3 in ff_hjkenc_send_frame (avctx=0x8404be0, frame=0x8408f70) at libavcodec/hjkenc.c:2139
#2  0x00007ffffdd53433 in avcodec_send_frame (avctx=0x8404be0, frame=0x8408f70) at libavcodec/encode.c:403
#3  0x00000000080012ae in encode_write (avctx=0x8404be0, frame=0x8408f70, fout=0x8404490) at doc/examples/hjkjpg_encode.c:83
#4  0x0000000008001af3 in main (argc=1, argv=0x7ffffffee168) at doc/examples/hjkjpg_encode.c:200
*/
static int hjk_EncEncodePicture(void *handle, HJK_ENC_PIC_PARAMS *params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HJK_ENC_REGISTER_RESOURCE *p_enc_reg_res = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    if (NULL == handle || NULL == params) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    printf("%d %s \n", __LINE__, __FUNCTION__);
    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_enc_reg_res = (HJK_ENC_REGISTER_RESOURCE *)params->inputBuffer;
    if (NULL == p_enc_reg_res) {
        return HJK_ENC_SUCCESS;
    }
    printf("%d %s in buf p_enc_reg_res->resourceToRegister=%p\n", __LINE__, __FUNCTION__, p_enc_reg_res->resourceToRegister);
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)params->outputBitstream; 
    printf("%d %s out buf params->outputBitstream=%p\n", __LINE__, __FUNCTION__,
           params->outputBitstream);

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
static int hjk_EncLockBitstream(void *handle, HJK_ENC_LOCK_BITSTREAM *lock_params)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == lock_params) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    if (NULL == lock_params->outputBitstream) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)lock_params->outputBitstream;
    printf("%d %s outputBitstream=%p  m_enc_out_virtual_addr=%p\n", __LINE__, __FUNCTION__, lock_params->outputBitstream, p_hjk_enc_inter_out->m_enc_out_virtual_addr);
    lock_params->bitstreamBufferPtr = p_hjk_enc_inter_out->m_enc_out_virtual_addr;
    lock_params->bitstreamSizeInBytes = p_hjk_enc_inter_out->m_enc_data_len;
    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnlockBitstream(void *handle, HJK_ENC_OUTPUT_PTR output_surface)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == output_surface) {
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    p_hjk_enc_info = p_hjk_enc_info;
    p_hjk_enc_inter_out = (HjkEncoderInterOut *)output_surface;
    p_hjk_enc_inter_out->m_enc_data_len = 0;

    return HJK_ENC_SUCCESS;
}

static int hjk_EncUnmapInputResource(
	void *handle, HJK_ENC_INPUT_PTR mappedResource)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == mappedResource) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)handle;
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

static int hjk_EncUnregisterResource(void *handle,
								HJK_ENC_REGISTERED_PTR regptr)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    int i = 0;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle || NULL == regptr) {
        return HJK_ENC_ERR_INVALID_PTR;
    }

    p_hjk_enc_info = (HjkEncoderInfo *)handle;
    for (i = 0; i < HJK_ENC_REGI_RES_NUM; i++) {
        if (p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource ==
            regptr) {
            // find ok, release i resource
            printf("%d %s find ok, release i resource\n", __LINE__,
                   __FUNCTION__);
            p_hjk_enc_info->m_list_enc_reg_res[i].registeredResource = NULL;
            break;
        }
    }

    return HJK_ENC_SUCCESS;
}
static int hjk_EncDestroyBitstreamBuffer(void *handle,
									HJK_ENC_OUTPUT_PTR output_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL != output_surface) {
        HjkEncoderInterOut *p_hjk_enc_inter_out = NULL;
        p_hjk_enc_inter_out = output_surface;
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

static int hjk_EncDestroyEncoder(void *handle)
{
    HjkEncoderInfo *p_hjk_enc_info = NULL;
    printf("%d %s \n", __LINE__, __FUNCTION__);

    if (NULL == handle) {
        printf("%d %s \n", __LINE__, __FUNCTION__);
        return HJK_ENC_ERR_INVALID_PTR;
    }
    p_hjk_enc_info = (HjkEncoderInfo *)handle;

    free(p_hjk_enc_info);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncDestroyInputBuffer(void *handle,
								HJK_ENC_INPUT_PTR input_surface)
{
    printf("%d %s \n", __LINE__, __FUNCTION__);

    return HJK_ENC_SUCCESS;
}

static int hjk_EncGetLastErrorString(void *handle)
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
    .hjkEncSetIOCudaStreams = hjk_EncSetIOCudaStreams,
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