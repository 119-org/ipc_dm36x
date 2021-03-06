
#ifndef _DRV_IMGS_MT9P031_5MP_H_
#define _DRV_IMGS_MT9P031_5MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (0)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0)
#define IMGS_V_PAD          (4)

#define IMGS_W_MAX          (2592)
#define IMGS_H_MAX          (1944)

#define IMGS_MHZ            (1000000L)
#define IMGS_PLL_REF_CLK    (24*IMGS_MHZ)

#define IMGS_CHIP_ID        (0x1801)

#define CHIP_VERSION        (0x00)
#define ROW_START           (0x01)
#define COL_START           (0x02)
#define ROW_SIZE            (0x03)
#define COL_SIZE            (0x04)
#define HORZ_BLANK          (0x05)
#define VERT_BLANK          (0x06)
#define OUTPUT_CTRL         (0x07)
#define SHUTTER_WIDTH_H     (0x08)
#define SHUTTER_WIDTH_L     (0x09)
#define PIXEL_CLK_CTRL      (0x0a)
#define RESTART             (0x0b)
#define SHUTTER_DELAY       (0x0c)
#define RESET               (0x0d)
#define PLL_CTRL            (0x10)
#define PLL_CFG1            (0x11)
#define PLL_CFG2            (0x12)
//#define READ_MODE_1         (0x1e)
#define READ_MODE_2         (0x20)
#define ROW_ADDR_MODE       (0x22)
#define COL_ADDR_MODE       (0x23)
#define GAIN_G1             (0x2b)
#define GAIN_B              (0x2c)
#define GAIN_R              (0x2d)
#define GAIN_G2             (0x2e)
#define GAIN_GLOBAL         (0x35)
#define ROW_BLK_DEF_OFFSET  (0x4B)

#define IMGS_I2C_ADDR       (gImgsI2CAddr) //(0xBA)
#define IMGS_RESET_GPIO     (80)

typedef struct
{
    int fps;        // Hz
    float t_frame;  // ns
    float t_row;    // ns
    float t_pclk;   // ns
    int W;
    int H;
    int SW;
    int SD;
    int HB;
    int VB;
    int HBmin;
    int VBmin;
    int f_pclk;     // Hz

    float shutterOverhead;

    int col_size;
    int row_size;
    int col_skip;
    int row_skip;
    int col_bin;
    int row_bin;
    int col_start;
    int row_start;

    int pll_M;
    int pll_N;
    int pll_DIV;
} DRV_ImgsFrameTime;

typedef struct
{
    DRV_I2cHndl i2cHndl;
    DRV_ImgsFrameTime curFrameTime;
    DRV_ImgsModeConfig curModeConfig;
} DRV_ImgsObj;


extern DRV_ImgsObj gDRV_imgsObj;

extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2_VS;

// for TNF+SNF cases
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2_VS;

extern DRV_ImgsIsifConfig   gDRV_imgsIsifConfig_Common_MT9P031;

extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Vnfdemo_MT9P031;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Common_MT9P031;

extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Appro_MT9P031;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_TI_MT9P031;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Common_MT9P031;

int DRV_imgsReset_MT9P031();
int DRV_imgsCalcFrameTime_MT9P031(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable);
int DRV_imgsCalcSW_MT9P031(int exposureTimeInUsecs);
int DRV_imgsCalcAgain_MT9P031(int aGain);
int DRV_imgsCalcFrameRate_MT9P031(int fps);
int DRV_imgsGetEshutter_MT9P031(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain_MT9P031(int *again);
int DRV_imgsCheckId_MT9P031();
int DRV_imgsSetRegs_MT9P031();

#endif

