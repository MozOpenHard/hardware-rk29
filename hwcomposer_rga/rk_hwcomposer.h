/****************************************************************************
*
*    Copyright (c) 2005 - 2011 by Vivante Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Vivante Corporation. This is proprietary information owned by
*    Vivante Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Vivante Corporation.
*
*****************************************************************************
*
*    Auto-generated file on 12/13/2011. Do not edit!!!
*
*****************************************************************************/




#ifndef __gc_hwcomposer_h_
#define __gc_hwcomposer_h_

/* Set 0 to enable LOGV message. See cutils/log.h */
#include <cutils/log.h>

#include <hardware/hwcomposer.h>
//#include <ui/android_native_buffer.h>

#include <hardware/rga.h>
#include <utils/Thread.h>
#include <linux/fb.h>

#define hwcDEBUG 0
#define hwcUseTime 0
#define hwcBlitUseTime 0
#define hwcDumpSurface 1
#define  ENABLE_HWC_WORMHOLE     1
#define  DUMP_SPLIT_AREA   0
#define FB1_IOCTL_SET_YUV_ADDR	0x5002
#define RK_FBIOSET_VSYNC_ENABLE     0x4629
//#define USE_LCDC_COMPOSER
#define USE_HW_VSYNC        1
#define FBIOSET_OVERLAY_STATE     	0x5018
/* Set it to 1 to enable swap rectangle optimization;
 * Set it to 0 to disable. */
/* Set it to 1 to enable pmem cache flush.
 * For linux kernel 3.0 later, you may not be able to flush PMEM cache in a
 * different process (surfaceflinger). Please add PMEM cache flush in gralloc
 * 'unlock' function, which will be called in the same process SW buffers are
 * written/read by software (Skia) */

#ifdef __cplusplus
extern "C" {
#endif

struct private_handle_t;
#ifdef TARGET_BOARD_PLATFORM_RK30XXB
#define private_handle_t IMG_native_handle_t
#define format iFormat
#define width iWidth
#define height iHeight
#endif

#if PLATFORM_SDK_VERSION >= 17
#define  hwc_composer_device_t 	hwc_composer_device_1_t
#define  hwc_layer_t           	hwc_layer_1_t

#define  hwc_composer_device 	hwc_composer_device_1
#define  hwc_layer           	hwc_layer_1
#define  hwc_layer_list_t	 	hwc_display_contents_1_t
#endif
enum
{
    /* NOTE: These enums are unknown to Android.
     * Android only checks against HWC_FRAMEBUFFER.
     * This layer is to be drawn into the framebuffer by hwc blitter */
    //HWC_TOWIN0 = 0x10,
    //HWC_TOWIN1,
    HWC_BLITTER = 100,
    HWC_DIM,
    HWC_CLEAR_HOLE
};


typedef enum _hwcSTATUS
{
	hwcSTATUS_OK					= 	 0,
	hwcSTATUS_INVALID_ARGUMENT      =   -1,
	hwcSTATUS_IO_ERR 			    = 	-2,
	hwcRGA_OPEN_ERR                 =   -3,
	hwcTHREAD_ERR                   =   -4,
	hwcMutex_ERR                    =   -5,

}
hwcSTATUS;

typedef struct _hwcRECT
{
    int                    left;
    int                    top;
    int                    right;
    int                    bottom;
}
hwcRECT;

#define IN
#define OUT

/* Area struct. */
struct hwcArea
{
    /* Area potisition. */
    hwcRECT                          rect;

    /* Bit field, layers who own this Area. */
    int                        owners;

    /* Point to next area. */
    struct hwcArea *                 next;
};


/* Area pool struct. */
struct hwcAreaPool
{
    /* Pre-allocated areas. */
    hwcArea *                        areas;

    /* Point to free area. */
    hwcArea *                        freeNodes;

    /* Next area pool. */
    hwcAreaPool *                    next;
};


typedef struct _hwcContext
{
    hwc_composer_device_t device;

    /* Reference count. Normally: 1. */
    unsigned int reference;

    /* GC state goes below here */

    /* Raster engine */
    int   engine_fd;
    /* Feature: 2D PE 2.0. */
    /* Base address. */
    unsigned int baseAddress;

    /* Framebuffer stuff. */
    int       fbFd;
    int       fbFd1;
    int       vsync_fd;
    int       fbWidth;
    int       fbHeight;
    bool      fb1_cflag;
    char      cupcore_string[16];

    hwc_procs_t *procs;

    pthread_t hdmi_thread;
    pthread_mutex_t lock;
    nsecs_t         mNextFakeVSync;
    float           fb_fps;
    unsigned int fbPhysical;
    unsigned int fbStride;
    /* PMEM stuff. */
    unsigned int pmemPhysical;
    unsigned int pmemLength;
#if ENABLE_HWC_WORMHOLE
    /* Splited composition area queue. */
    hwcArea *                        compositionArea;

    /* Pre-allocated area pool. */
    hwcAreaPool                      areaPool;
#endif
     int      flag;

}
hwcContext;

#define hwcMIN(x, y)			(((x) <= (y)) ?  (x) :  (y))
#define hwcMAX(x, y)			(((x) >= (y)) ?  (x) :  (y))

#define hwcIS_ERROR(status)			(status < 0)


#define _hwcONERROR(prefix, func) \
    do \
    { \
        status = func; \
        if (hwcIS_ERROR(status)) \
        { \
            LOGD( "ONERROR: status=%d @ %s(%d) in ", \
                status, __FUNCTION__, __LINE__); \
            goto OnError; \
        } \
    } \
    while (false)
#define hwcONERROR(func)            _hwcONERROR(hwc, func)

#ifdef  ALOGD
#define LOGV        ALOGV
#define LOGE        ALOGE
#define LOGD        ALOGD
#define LOGI        ALOGI
#endif
/******************************************************************************\
 ********************************* Blitters ***********************************
\******************************************************************************/

/* 2D blit. */
hwcSTATUS
hwcBlit(
    IN hwcContext * Context,
    IN hwc_layer_t * Src,
    IN struct private_handle_t * DstHandle,
    IN hwc_rect_t * SrcRect,
    IN hwc_rect_t * DstRect,
    IN hwc_region_t * Region
    );


hwcSTATUS
hwcDim(
    IN hwcContext * Context,
    IN hwc_layer_t * Src,
    IN struct private_handle_t * DstHandle,
    IN hwc_rect_t * DstRect,
    IN hwc_region_t * Region
    );

hwcSTATUS
hwcLayerToWin(
    IN hwcContext * Context,
    IN hwc_layer_t * Src,
    IN struct private_handle_t * DstHandle,
    IN hwc_rect_t * SrcRect,
	IN hwc_rect_t * DstRect,
    IN hwc_region_t * Region,
    IN int Index,
    IN int Win
    );
hwcSTATUS
hwcClear(
    IN hwcContext * Context,
    IN unsigned int Color,
    IN struct private_handle_t * DstHandle,
    IN hwc_rect_t * DstRect,
    IN hwc_region_t * Region
    );


/******************************************************************************\
 ************************** Native buffer handling ****************************
\******************************************************************************/

hwcSTATUS
hwcGetFormat(
    IN  struct private_handle_t * Handle,
    OUT RgaSURF_FORMAT * Format
    );

hwcSTATUS
hwcLockBuffer(
    IN  hwcContext *  Context,
    IN  struct private_handle_t * Handle,
    OUT void * *  Logical,
    OUT unsigned int* Physical,
    OUT unsigned int* Width,
    OUT unsigned int* Height,
    OUT unsigned int* Stride,
    OUT void * *  Info
    );


hwcSTATUS
hwcUnlockBuffer(
    IN hwcContext * Context,
    IN struct private_handle_t * Handle,
    IN void * Logical,
    IN void * Info,
    IN unsigned int  Physical
    );

int
_HasAlpha(RgaSURF_FORMAT Format);

int closeFb(int fd);
int  getHdmiMode();
void init_hdmi_mode();
/******************************************************************************\
 ****************************** Rectangle split *******************************
\******************************************************************************/
/* Split rectangles. */
bool
hwcSplit(
    IN  hwcRECT * Source,
    IN  hwcRECT * Dest,
    OUT hwcRECT * Intersection,
    OUT hwcRECT * Rects,
    OUT int  * Count
    );

hwcSTATUS
WormHole(
    IN hwcContext * Context,
    IN hwcRECT * Rect
    );

void
_FreeArea(
    IN hwcContext * Context,
    IN hwcArea* Head
    );

void
_SplitArea(
    IN hwcContext * Context,
    IN hwcArea * Area,
    IN hwcRECT * Rect,
    IN int Owner
    );

hwcArea *
_AllocateArea(
    IN hwcContext * Context,
    IN hwcArea * Slibing,
    IN hwcRECT * Rect,
    IN int Owner
    );



extern "C" int clock_nanosleep(clockid_t clock_id, int flags,
                           const struct timespec *request,
                           struct timespec *remain);

#ifdef __cplusplus
}
#endif

#endif /* __gc_hwcomposer_h_ */

