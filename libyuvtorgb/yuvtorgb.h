#ifndef YUVTORGB_H
#define YUVTORGB_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	int yuvAddr;//phyaddr
	int outAddr;//phyaddr, here do not flush, do this yourself.
	int inwidth;//align 16
	int inheight;//align 16
	int inColor;
	int outColor;
	int outwidth;//align 8
	int outheight;//align 8
	int outStrideX; //align 4pixel(AXI data-bus 64bit)
	int inCropW;
	int inCropH;
	int rotation;
}YUV2RGBParams;

#define PP_IN_YUV420sp 1 //the same in Postprocess.h
#define PP_IN_YUV420_P 2

#define PP_OUT_RGB565	0
#define PP_OUT_ARGB		1
#define PP_OUT_ABGR		2

#define PP_ROTATION_NONE                                0U
#define PP_ROTATION_RIGHT_90                            1U
#define PP_ROTATION_LEFT_90                             2U
#define PP_ROTATION_HOR_FLIP                            3U
#define PP_ROTATION_VER_FLIP                            4U
#define PP_ROTATION_180                                 5U


int doYuvToRgb(YUV2RGBParams *param);

//the following must be hidden
int yuv2rgb_open();
int yuv2rgb_perform(int fd, YUV2RGBParams *params, int wait);//do not use in above android2.3
int yuv2rgb_sync(int fd);
int yuv2rgb_close(int fd);

#ifdef __cplusplus
}
#endif
#endif
