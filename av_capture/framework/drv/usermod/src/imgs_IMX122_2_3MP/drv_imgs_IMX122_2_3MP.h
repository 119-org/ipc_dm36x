
#ifndef _DRV_IMGS_IMX122_2_3MP_H_
#define _DRV_IMGS_IMX122_2_3MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>
#include <drv_spi.h>

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (200)  //(104)
#define IMGS_V_PAD          (36)  //(2)

#define IMGS_H_3M_PAD	    (104)
#define IMGS_V_3M_PAD	    (52)		//56

#define IMGS_W_MAX          (2200)
#define IMGS_H_MAX          (1608)

  
#define IMGS_MHZ            (1000000L)

//#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define IMGS_CHIP_PID       (0x9711)
#define IMGS_CHIP_MID       (0x7FA2)

#define IMGS_I2C_ADDR       (0x5A)
#define IMGS_RESET_GPIO	    (80)	

#define IMGS_SPI_ADDR		   (0x02)
#define IMGS_SPI_ADDR01	       (0x03)
#define IMGS_LIGHT_SENSOR	   (45)
#define IMGS_LIGHT_SENSOR_LINK (45)
#define IMGS_ND_CTRL1		   (46)
#define IMGS_ND_CTRL2		   (49)

#define IMGS_SENSOR_LINEWIDTH		(1125)



#define RegBaseVirtStart	(0x01C00000)

#define PINMUX_BASE		(0x01C40000)
#define PINMUX_LENGTH	(0x8C)
#define PINMUX0_OFSET	(0x0000)
#define PINMUX1_OFSET	(0x0004)
#define PINMUX2_OFSET	(0x0008)
#define PINMUX3_OFSET	(0x000C)
#define PINMUX4_OFSET	(0x0010)

/*
#define IMGS_APCHIP_C1_GIO	(93)
#define AP_CHIP_C1_PINMUX	(PINMUX_BASE+PINMUX0_OFSET)
#define AP_CHIP_C1_MASK	(((unsigned int)0x3)<<14)
#define AP_CHIP_C1_VAL 		(((unsigned int)0x1)<<14)

#define IMGS_APCHIP_D1_GIO	(90)
#define AP_CHIP_D1_PINMUX	(PINMUX_BASE+PINMUX1_OFSET)
#define AP_CHIP_D1_MASK	(((unsigned int)0x3)<<4)
#define AP_CHIP_D1_VAL 		(((unsigned int)0x0)<<4)


#define _PWM_BASE_			(0x1C22000)
#define _PWM0_BASE_		(_PWM_BASE_)
#define _PWM1_BASE_		(_PWM_BASE_+0x400)
#define _PWM2_BASE_		(_PWM_BASE_+0x800)
#define _PWM3_BASE_		(_PWM_BASE_+0xC00)

#define _PWM_PCR_			(0x04)
#define _PWM_START_		(0x0C)
#define _PWM_CFG_			(0x08)
#define _PWM_PER_			(0x14)
#define _PWM_PH1D_			(0x18)
*/
/* PINMUX1 GIO92 -> PWM0 for Video */
/*
#define IMGS_AutoIris_VIDEO_PWM_REG 	(_PWM0_BASE_)
#define IMGS_AutoIris_VIDEO_GIO			(92)
#define AP_AutoIris_VIDEO_PINMUX		(PINMUX_BASE+PINMUX1_OFSET)
#define AP_AutoIris_VIDEO_MASK			(((unsigned int)0x3)<<0)
#define AP_AutoIris_VIDEO_VAL 			(((unsigned int)0x2)<<0)
*/
/* PINMUX1 GIO91 -> PWM1 for DC */
/*
#define IMGS_AutoIris_DC_PWM_REG 		(_PWM1_BASE_)
#define IMGS_AutoIris_DC_GIO			(91)
#define AP_AutoIris_DC_PINMUX			(PINMUX_BASE+PINMUX1_OFSET)
#define AP_AutoIris_DC_MASK				(((unsigned int)0x3)<<2)
#define AP_AutoIris_DC_VAL 				(((unsigned int)0x2)<<2)
*/

typedef struct {

  int fps;        // Hz
  int Is50Hz;
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
  int pll_PREDIV;

} DRV_ImgsFrameTime;

typedef struct {

  DRV_I2cHndl i2cHndl;
  DRV_SPIHndl spiHndl;
  DRV_ImgsFrameTime curFrameTime;
  DRV_ImgsModeConfig curModeConfig;

} DRV_ImgsObj;


extern DRV_ImgsObj gDRV_imgsObj;

extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2_VS_IMX122;

// for TNF+SNF cases
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1_VS_IMX122;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2_VS_IMX122;

extern DRV_ImgsIsifConfig   gDRV_imgsIsifConfig_Common_IMX122;

extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Vnfdemo_IMX122;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Common_IMX122;

extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Appro_IMX122;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_TI_IMX122;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Common_IMX122;

int DRV_imgsReset_IMX122();

int DRV_imgsCalcFrameTime_IMX122(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable);
int DRV_imgsCalcSW_IMX122(int exposureTimeInUsecs);
int DRV_imgsCalcAgain_IMX122(int aGain);
int DRV_imgsCalcFrameRate_IMX122(int fps);
int DRV_imgsGetEshutter_IMX122(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain_IMX122(int *again);
int DRV_imgsSetDgain_IMX122(int dgain);

int DRV_imgsCheckId_IMX122(DRV_SPIHndl *hndl);

//int DRV_imgs_APChipInit_IMX122(int *pGioC1, int *pGioD1);

#endif

