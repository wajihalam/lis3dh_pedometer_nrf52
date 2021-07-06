#include "lis3dh.h"

void LIS3DH_Init(void)
{
    uint8_t chipid = 0;
    uint8_t tmp = 0;

    for(int i=0; i<30000; i++){__ASM("nop");}

    //read chip id
    for (int i = 0; i < 10 ; i++ ) 
    {
        for(int j = 0 ; j < 1000 ; j++ ) {__asm("NOP");}
        i2c_read(LIS3DH_I2C_ADDRESS ,LIS3DH_REGISTER_WHO_AM_I, &chipid, 0x01);
        if ( chipid == 0x33 ) {
            break;
        }
    }
    
    //reboot sensor
    tmp = (LIS3DH_CTRL_REG5_REBOOT);
    i2c_write(LIS3DH_I2C_ADDRESS ,LIS3DH_REGISTER_CTRL_REG5, &tmp, 0x01);
    
    //delay some time
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    
    //report rate = 200Hz, x/y/z axis enable
    tmp = (LIS3DH_CTRL_REG1_DATARATE_200HZ | LIS3DH_CTRL_REG1_XYZEN);
    i2c_write(LIS3DH_I2C_ADDRESS ,LIS3DH_REGISTER_CTRL_REG1, &tmp, 0x01);
    
    //delay some time
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    for(int i=0; i<30000; i++){__ASM("nop");}
    
    //enable block data update, full-scale = +-8g
    tmp = (LIS3DH_CTRL_REG4_BLOCKDATAUPDATE | LIS3DH_CTRL_REG4_SCALE_8G);
    i2c_write(LIS3DH_I2C_ADDRESS ,LIS3DH_REGISTER_CTRL_REG4, &tmp, 0x01);
}

void LIS3DH_GetAccData(float *acc)
{
    uint8_t tmp[6] = {0};
    int16_t tmp_int16[3] = {0};
    uint8_t status = 0;
    
    //check data ready?
    i2c_read(LIS3DH_I2C_ADDRESS ,(LIS3DH_REGISTER_STATUS_REG2), &status, 0x01);
    if(status & LIS3DH_STATUS_REG_ZYXDA)
    {    
        i2c_read(LIS3DH_I2C_ADDRESS ,(LIS3DH_REGISTER_OUT_X_L | LIS3DH_READ_MULTI_BYTES), tmp, 0x06);
        tmp_int16[0] = (int16_t)((tmp[1]<< 8) | tmp[0]);
        tmp_int16[1] = (int16_t)((tmp[3]<< 8) | tmp[2]);
        tmp_int16[2] = (int16_t)((tmp[5]<< 8) | tmp[4]);

        acc[0] = (float)(tmp_int16[0]) * LIS3DH_ACC_CONVERT_8G;
        acc[1] = (float)(tmp_int16[1]) * LIS3DH_ACC_CONVERT_8G;
        acc[2] = (float)(tmp_int16[2]) * LIS3DH_ACC_CONVERT_8G;
    }
}
