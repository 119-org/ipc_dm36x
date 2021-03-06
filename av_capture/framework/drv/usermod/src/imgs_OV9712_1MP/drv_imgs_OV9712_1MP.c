
#include "drv_imgs_OV9712_1MP.h"
#include <drv_gpio.h>

DRV_ImgsObj gDRV_imgsObj;

int DRV_imgsCheckId_OV9712();
int DRV_imgsSetFramerate_OV9712(int fps);
int DRV_imgsEnable_OV9712(Bool enable);

int DRV_imgsOpen_OV9712(DRV_ImgsConfig *config)
{
    int status, retry = 10;
    Uint16 width, height;

    memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

    DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

    width  += 0;//IMGS_H_PAD;
    height += 0;//IMGS_V_PAD;

    DRV_imgsCalcFrameTimes_OV9712(config->fps, width, height);

    status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cOpen()\n");
        return status;
    }

//#ifdef BOARD_AP_IPNC
    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioSet(IMGS_RESET_GPIO);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(50);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(50);
//#endif

    do
    {
        status = DRV_imgsCheckId_OV9712();
        if (status == OSA_SOK)
        {
            break;
        }
        OSA_waitMsecs(10);
    } while (retry--);

    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_imgsCheckId()\n");
        DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
        return status;
    }

    return 0;
}

int DRV_imgsClose_OV9712()
{
    int status = DRV_imgsEnable_OV9712(FALSE);
    status |= DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
    return status;
}

char* DRV_imgsGetImagerName_OV9712()
{
    return "OV9712";
}

int DRV_imgsSpecificSetting_OV9712(void)
{
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
    Uint8 regAddr[128];
    Uint8 regValue[128];
    int status = 0;
    int j = 0;
    int i = 0;

    regAddr[i]  = 0x12;  regValue[i] = 0x80;  i++; //com7 soft reset 0x80
    
    regAddr[i]  = 0x1e;  regValue[i] = 0x07;  i++; //com11 reserved
    regAddr[i]  = 0x5f;  regValue[i] = 0x18;  i++; //reserved
    regAddr[i]  = 0x69;  regValue[i] = 0x04;  i++; //reserved
    regAddr[i]  = 0x65;  regValue[i] = 0x2a;  i++; //reserved
    regAddr[i]  = 0x68;  regValue[i] = 0x0a;  i++; //reserved
    regAddr[i]  = 0x39;  regValue[i] = 0x28;  i++; //reserved
    regAddr[i]  = 0x4d;  regValue[i] = 0x90;  i++; //reserved
    regAddr[i]  = 0xc1;  regValue[i] = 0x80;  i++; //YAVG CTRL
    regAddr[i]  = 0x0c;  regValue[i] = 0x30;  i++; //com3 single output
    regAddr[i]  = 0x6d;  regValue[i] = 0x02;  i++; //reserved
    regAddr[i]  = 0xbc;  regValue[i] = 0x68;  i++; //WBC ctr : default value
    regAddr[i]  = 0x12;  regValue[i] = 0x00;  i++; //reset
    regAddr[i]  = 0x3b;  regValue[i] = 0x00;  i++; //reserved
    regAddr[i]  = 0x97;  regValue[i] = 0x80;  i++; //dsp ctr : default value
    regAddr[i]  = 0x17;  regValue[i] = 0x25;  i++; //HSSTART
    regAddr[i]  = 0x18;  regValue[i] = 0xa2;  i++; //AHSIZE
    regAddr[i]  = 0x19;  regValue[i] = 0x01;  i++; //VSTART
    regAddr[i]  = 0x03;  regValue[i] = 0x0a;  i++; //reg
    regAddr[i]  = 0x32;  regValue[i] = 0x07;  i++; //reg
    regAddr[i]  = 0x98;  regValue[i] = 0x00;  i++; //dsp
    regAddr[i]  = 0x99;  regValue[i] = 0x28;  i++; //dsp
    regAddr[i]  = 0x9a;  regValue[i] = 0x00;  i++; //dsp
    regAddr[i]  = 0x57;  regValue[i] = 0x00;  i++; //dsp
    regAddr[i]  = 0x59;  regValue[i] = 0xa0;  i++; //dsp
    regAddr[i]  = 0x4c;  regValue[i] = 0x13;  i++; //reserved
    regAddr[i]  = 0x4b;  regValue[i] = 0x36;  i++; //reserved
    regAddr[i]  = 0x3d;  regValue[i] = 0x3c;  i++; //RENDL : default value
    regAddr[i]  = 0x3e;  regValue[i] = 0x03;  i++; //RENDH: default value
    regAddr[i]  = 0xbd;  regValue[i] = 0xa0;  i++; //YACG
    regAddr[i]  = 0xbe;  regValue[i] = 0xb4;  i++; //YACG
    regAddr[i]  = 0xb5;  regValue[i] = 0x00;  i++; //LENC ctr
    regAddr[i]  = 0x4e;  regValue[i] = 0x55;  i++; //average section weighting
    regAddr[i]  = 0x4f;  regValue[i] = 0x55;  i++; //average section weighting
    regAddr[i]  = 0x50;  regValue[i] = 0x55;  i++; //average section weighting
    regAddr[i]  = 0x51;  regValue[i] = 0x55;  i++; //average section weighting
    regAddr[i]  = 0x24;  regValue[i] = 0x55;  i++; //WPT
    regAddr[i]  = 0x25;  regValue[i] = 0x40;  i++; //BPT
    regAddr[i]  = 0x26;  regValue[i] = 0xa1;  i++; //VPT
    
    regAddr[i]  = 0x5c;  regValue[i] = 0x59;  i++; //PLL pre-divider 0x59
    regAddr[i]  = 0x5d;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0x11;  regValue[i] = (pFrame->pll_div >> 1) - 1;  i++; 
    regAddr[i]  = 0x2a;  regValue[i] = pFrame->cols & 0xff;  i++;
    regAddr[i]  = 0x2b;  regValue[i] = (pFrame->cols >> 8) & 0xff;  i++;  
    
    regAddr[i]  = 0x2d;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0x2e;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0x13;  regValue[i] = 0x80;  i++; //common control 8 bit 0:AEC ctr
    regAddr[i]  = 0x14;  regValue[i] = 0x40;  i++;
    regAddr[i]  = 0x38;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0xb6;  regValue[i] = 0x08;  i++;
    regAddr[i]  = 0x96;  regValue[i] = 0xe1;  i++;
    regAddr[i]  = 0x01;  regValue[i] = 0x40;  i++;
    regAddr[i]  = 0x02;  regValue[i] = 0x40;  i++;
    regAddr[i]  = 0x05;  regValue[i] = 0x40;  i++;
    regAddr[i]  = 0x06;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0x07;  regValue[i] = 0x00;  i++;
    regAddr[i]  = 0x41;  regValue[i] = 0x80;  i++;
    regAddr[i]  = 0x37;  regValue[i] = 0x06;  i++;

    //regAddr[i]  = 0x3c;  regValue[i] = 0xff;  i++; 
    //regAddr[i]  = 0x1f;  regValue[i] = 0xff;  i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

#if 0
    for (j = 0; j < i; j++)
    {
        status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr[j], &regValue[j], 1);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2cRead8()\n");
            return status;
        }
    }
#endif

    return status;
}

int DRV_imgsSet50_60Hz_OV9712(Bool is50Hz)
{
    int fps;
    if (gDRV_imgsObj.curFrameTime.fps == 30 || gDRV_imgsObj.curFrameTime.fps == 25)
    {
        if (is50Hz)
        {
            fps = 25;
        }
        else
        {
            fps = 30;
        }
        DRV_imgsSetFramerate_OV9712(fps);
    }
    return 0;
}

int DRV_imgsSetFramerate_OV9712(int fps)
{
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
    Uint8 regAddr[3];
    Uint8 regValue[3];
    int i = 0, status = 0;

    DRV_imgsCalcFrameRate_OV9712(fps);
    i = 0;

    regAddr[i]  = 0x11;  regValue[i] = (pFrame->pll_div >> 1) - 1;  i++; 
    regAddr[i]  = 0x2a;  regValue[i] = pFrame->cols & 0xff;  i++;
    regAddr[i]  = 0x2b;  regValue[i] = (pFrame->cols >> 8) & 0xff;  i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    return status;
}

int DRV_imgsBinEnable_OV9712(Bool enable)
{
    return 0;
}

int DRV_imgsBinMode_OV9712(int binMode)
{
    return 0;
}

int DRV_imgsReverseMode_OV9712()
{
    return 0;
}

int DRV_imgsFlipMode_OV9712(int enable)
{
    Uint8 regAddr[4];
    Uint8 regValue[4],regVal_old;
    int i = 0, status;

    regAddr[i]  = 0x04;
    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal_old, 1);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }

    if (enable == 0)
        regValue[i] = (regVal_old & 0xBf) | 0<<6;
    else if (enable == 1)
        regValue[i] = (regVal_old & 0xBf) | 1<<6;

    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsMirrorMode_OV9712(int enable)
{
    Uint8 regAddr[4];
    Uint8 regValue[4],regVal_old;
    int i = 0, status;

    regAddr[i]  = 0x04;
    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal_old, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }

    if (enable == 0)
    {
        regValue[i] = (regVal_old & 0x7f) | 0<<7;
    }
    else if (enable == 1)
    {
        regValue[i] = (regVal_old & 0x7f) | 1<<7;
    }

    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsSetMirror_OV9712(Bool flipH, Bool flipV)
{
    int status = OSA_SOK;

    if (flipH) 
    {
        status = DRV_imgsMirrorMode_OV9712(1);   
    }
    else
    {
        status = DRV_imgsMirrorMode_OV9712(0);     
    }

    if (flipV) 
    {
        status = DRV_imgsFlipMode_OV9712(1);    
    }
    else
    {
        status = DRV_imgsFlipMode_OV9712(0);   
    }
    
    return status;
}

int DRV_imgsSetBlc_OV9712(int value)
{
    Uint8 regVal, hvalue;
    Uint8 regAddr = 0x37;
    int status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal, 1);
    regVal |= 0x04;
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    regAddr = 0x43;
    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal, 1);
    regVal |= 0xC0;
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    regAddr = 0x48;
    hvalue = value % 256;
    regVal = hvalue;
    regVal = regVal << 2 + hvalue;
    regVal = regVal << 2 + hvalue;
    regVal = regVal << 2 + hvalue;
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regVal, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsSetAgainExposureGroupLatch_OV9712(int again, Uint32 eshutter, int setRegDirect)
{
    Uint8 regAddr;
    Uint8 regValue;
    int status;
    /* start group latch */
    regAddr = 0x04;
    regValue = 0x09;
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    status |= DRV_imgsSetEshutter_OV9712(eshutter, setRegDirect);
    status |= DRV_imgsSetAgain_OV9712(again, setRegDirect);
    regAddr = 0x04;
    regValue = 0x00;
    status |= DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xff;
    regValue = 0xff;
    status |= DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    return status;

}

int DRV_imgsSetDgain_OV9712(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain_OV9712(int again, int setRegDirect)
{
    Uint8 regAddr;
    Uint16 regValue;
    int status;

    regAddr = GAIN;

    if (setRegDirect)
    {
        regValue = again;
    }
    else
    {
        regValue = DRV_imgsCalcAgain_OV9712(again);
    }

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsSetEshutter_OV9712(Uint32 eshutterInUsec, int setRegDirect)
{
    int status;
    int SW;
    Uint8 regAddr[2];
    Uint8 regValue[2];

    regAddr[0] = AEC;
    regAddr[1] = AECH;

    if (setRegDirect)
    {
        regValue[0] = eshutterInUsec & 0xff;
        regValue[1] = (eshutterInUsec>>8) & 0xff;
    }
    else
    {
        DRV_imgsCalcSW_OV9712(eshutterInUsec, &SW);
        regValue[0] = SW & 0xff;
        regValue[1] = (SW >> 8) & 0xff;
    }
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 2);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    //status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 2);
    //printf("\n\n AEC=%d, AECH=%d\n", regValue[0], regValue[1]);
    //printf("\n\n shutter=%d, SW=%d\n", eshutterInUsec, SW);

    return status;
}

//Gang: read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain_OV9712(int *again)
{
    Uint8 regAddr;
    Uint16 regValue;
    int status;

    regAddr = GAIN;
    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }

    *again = regValue & 0x7f;
    if (regValue & 0x40)
    {
        *again <<= 1;
    }

    return status;
}

int DRV_imgsGetEshutter_OV9712(Uint32 *eshutterInUsec)
{
    int status;
    Uint8 regAddr[2];
    Uint8 regValue[2];
    int  regvalue;

    regAddr[0] = AEC;
    regAddr[1] = AECH;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 2);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }

    regvalue = regValue[0] + (regValue[1] << 8);
    *eshutterInUsec = DRV_imgsInvertSW_OV9712(regvalue);

    return status;
}

int DRV_imgsSetDcSub_OV9712(Uint32 dcSub, int setRegDirect)
{
    return 0;
}

int DRV_imgsEnable_OV9712(Bool enable)
{
    Uint8 regAddr;
    Uint16 regValue;
    int status = OSA_SOK;

    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    if (enable)
    {
        status = DRV_imgsSetRegs_OV9712();
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_imgsSetRegs()\n");
            return status;
        }
        //DRV_imgsSpecificSetting();
        //DRV_imgsReverseMode();
    }
#if 0
    if ( (pFrame->f_pclk == (96*IMGS_MHZ))&&( pFrame->row_bin > 0 || pFrame->col_bin > 0))
    {
        regAddr  = OUTPUT_CTRL;
        regValue = 0x1f8C | ((enable & 0x1) << 1);
    }
    else
    {
        regAddr  = OUTPUT_CTRL;
        regValue = 0x1f80 | ((enable & 0x1) << 1);
    }

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }
    OSA_waitMsecs(10);
#endif
    return OSA_SOK;
}

DRV_ImgsModeConfig *DRV_imgsGetModeConfig_OV9712(int sensorMode)
{
    return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig *DRV_imgsGetIsifConfig_OV9712(int sensorMode)
{
    return &gDRV_imgsIsifConfig_Common_OV9712;
}

DRV_ImgsIpipeConfig *DRV_imgsGetIpipeConfig_OV9712(int sensorMode, int vnfDemoCfg)
{
    if (vnfDemoCfg)
        return &gDRV_imgsIpipeConfig_Vnfdemo_OV9712;
    else
        return &gDRV_imgsIpipeConfig_Common_OV9712;
}

DRV_ImgsH3aConfig *DRV_imgsGetH3aConfig_OV9712(int sensorMode, int aewbVendor)
{
    if (aewbVendor == 1)
    {
        printf("APPRO Setting Applied\n");
        return &gDRV_imgsH3aConfig_Appro_OV9712;
    }
    else if (aewbVendor == 2)
    {
        printf("TI Setting Applied\n");
        return &gDRV_imgsH3aConfig_TI_OV9712;
    }
    else
    {
        printf("Common Setting Applied\n");
        return &gDRV_imgsH3aConfig_Common_OV9712;
    }
}

DRV_ImgsLdcConfig *DRV_imgsGetLdcConfig_OV9712(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    sensorMode &= 0xFF;

    switch (sensorMode)
    {
    case DRV_IMGS_SENSOR_MODE_720x480:
        if (ldcInFrameWidth==864) return &gDRV_imgsLdcConfig_736x480_0_VS;
        if (ldcInFrameWidth==352) return &gDRV_imgsLdcConfig_736x480_1_VS;
        if (ldcInFrameWidth==736) return &gDRV_imgsLdcConfig_736x480_0;
        if (ldcInFrameWidth==288) return &gDRV_imgsLdcConfig_736x480_1;
        if (ldcInFrameWidth==768) return &gDRV_imgsLdcConfig_768x512_0;
        if (ldcInFrameWidth==320) return &gDRV_imgsLdcConfig_768x512_1;
        if (ldcInFrameWidth==928) return &gDRV_imgsLdcConfig_768x512_0_VS;
        if (ldcInFrameWidth==384) return &gDRV_imgsLdcConfig_768x512_1_VS;
        break;
    default:
    case DRV_IMGS_SENSOR_MODE_1280x720:
        if (ldcInFrameWidth==1280) return &gDRV_imgsLdcConfig_1280x736_0;
        if (ldcInFrameWidth==320)  return &gDRV_imgsLdcConfig_1280x736_1;
        if (ldcInFrameWidth==640)  return &gDRV_imgsLdcConfig_1280x736_2;
        if (ldcInFrameWidth==1536) return &gDRV_imgsLdcConfig_1280x736_0_VS;
        if (ldcInFrameWidth==384)  return &gDRV_imgsLdcConfig_1280x736_1_VS;
        if (ldcInFrameWidth==768)  return &gDRV_imgsLdcConfig_1280x736_2_VS;
        if (ldcInFrameWidth==1312) return &gDRV_imgsLdcConfig_1312x768_0;
        if (ldcInFrameWidth==352)  return &gDRV_imgsLdcConfig_1312x768_1;
        if (ldcInFrameWidth==672)  return &gDRV_imgsLdcConfig_1312x768_2;
        if (ldcInFrameWidth==1600) return &gDRV_imgsLdcConfig_1312x768_0_VS;
        if (ldcInFrameWidth==448)  return &gDRV_imgsLdcConfig_1312x768_1_VS;
        if (ldcInFrameWidth==832)  return &gDRV_imgsLdcConfig_1312x768_2_VS;
        break;
    }

    return NULL;
}

int DRV_imgsReset_OV9712()
{
    Uint8 regAddr[8];
    Uint16 regValue[8];
    int status, i;

#if 0
    i=0;

    regAddr[i]  = OUTPUT_CTRL;
    regValue[i] = 0x1f80;

    regAddr[i]  = RESET;
    regValue[i] = 0x1;
    i++;

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }

    if (status == OSA_SOK)
    {

        i=0;

        regAddr[i]  = RESET;
        regValue[i] = 0x0;
        i++;

        status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    }

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }
#endif
    return status;
}

int DRV_imgsCheckId_OV9712()
{
    int status;
    Uint8  regAddr;
    Uint8 regValue;
    Uint16 chipId;

    regAddr = PIDH;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }
    chipId = regValue;

    regAddr = PIDL;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }
    chipId = (chipId <<8) | regValue;

    //OSA_printf("----chipId =%x-----\n",chipId);
    if (chipId != IMGS_CHIP_ID)
        return OSA_EFAIL;

    return OSA_SOK;
}

int DRV_imgsSetRegs_OV9712()
{
    int i, status = 0;
    static Uint8 regAddr[32];
    static Uint16 regValue[32];

    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    OSA_waitMsecs(10);
    status = DRV_imgsSpecificSetting_OV9712();
    return status;
}

DRV_ImgsFuncs OV9712ImgsFuncs = 
{
    .imgsOpen            = DRV_imgsOpen_OV9712,
    .imgsClose           = DRV_imgsClose_OV9712,
    .imgsSetMirror       = DRV_imgsSetMirror_OV9712,
    .imgsGetImagerName   = DRV_imgsGetImagerName_OV9712,
    .imgsSetAgain        = DRV_imgsSetAgain_OV9712,
    .imgsSetDgain        = DRV_imgsSetDgain_OV9712,
    .imgsSetEshutter     = DRV_imgsSetEshutter_OV9712,
    .imgsSetDcSub        = DRV_imgsSetDcSub_OV9712,
    .imgsBinEnable       = DRV_imgsBinEnable_OV9712,
    .imgsBinMode         = DRV_imgsBinMode_OV9712,
    .imgsSetFramerate    = DRV_imgsSetFramerate_OV9712,
    .imgsSet50_60Hz      = DRV_imgsSet50_60Hz_OV9712, 
    .imgsEnable          = DRV_imgsEnable_OV9712,
    .imgsGetModeConfig   = DRV_imgsGetModeConfig_OV9712,
    .imgsGetIsifConfig   = DRV_imgsGetIsifConfig_OV9712,
    .imgsGetH3aConfig    = DRV_imgsGetH3aConfig_OV9712,
    .imgsGetIpipeConfig  = DRV_imgsGetIpipeConfig_OV9712,
    .imgsGetLdcConfig    = DRV_imgsGetLdcConfig_OV9712
};

