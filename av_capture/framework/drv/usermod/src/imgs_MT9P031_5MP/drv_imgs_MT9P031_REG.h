
#ifndef _DRV_IMGS_MT9P031_REG_H_
#define _DRV_IMGS_MT9P031_REG_H_

const unsigned short MT9P031_1080P[]=
{
//[1080P25fps]
     0x0D, 0x0001,   // RESET_REG
     0x0D, 0x0000,   // RESET_REG
    
     0x1E, 0x0006,   // READ_MODE_1
     0x4F, 0x0011,   // VLN_BIAS_CURRENT
     0x57, 0x0002,   // ANTI_ECLIPSE_VOLTAGE
    
     0x10, 0x0051,   // PLL_CONTROL
     0x11, 0x1801,   // PLL_CONFIG_1
     0x12, 0x0002,   // PLL_CONFIG_2
     0x10, 0x0053,   // PLL_CONTROL
    
     0x07, 0x1F8E,   // CONTROL_MODE_REG
     0x01, 0x01AF,   // ROW_WINDOW_START_REG
     0x02, 0x014C,   // COL_WINDOW_START_REG
     0x03, 0x0437,   // ROW_WINDOW_SIZE_REG
     0x04, 0x077F,   // COL_WINDOW_SIZE_REG
     0x05, 0x01C6,   // HORZ_BLANK_REG  
     0x06, 0x0114,   // VERT_BLANK_REG  Max 0x07FF
    
     0x22, 0x0000,   // ROW_MODE
     0x23, 0x0000,   // COL_MODE
    
     0x70, 0x0079,   // 
     0x71, 0x7800,   // 
     0x72, 0x7800,   // 
     0x73, 0x0300,   // 
     0x74, 0x0300,   // 
     0x75, 0x3C00,   // 
     0x76, 0x4E3D,   // 
     0x77, 0x4E3D,   // 
     0x78, 0x774F,   // 
     0x79, 0x7900,   // 
     0x7A, 0x7900,   // 
     0x7B, 0x7800,   // 
     0x7C, 0x7800,   // 
     0x7E, 0x7800,   // 
     0x7F, 0x7800,   // 
    
     0x29, 0x0481,   // 
     0x3F, 0x0007,   // 
     0x41, 0x0003,   // 
     0x48, 0x0018,   // 
     0x5F, 0x1C16,   // 
    
     0x08, 0x0000,   // SHUTTER_WIDTH_HI
     0x09, 0x009D,   // INTEG_TIME_REG
     0x0C, 0x0000,   // SHUTTER_DELAY_REG
    
     0x3E, 0x0007,   // When gain <=4x, set to 0x0007  (blooming fix); when  gain > 4x , set to 0x0087 (hot pixels optimization)
};

#endif
