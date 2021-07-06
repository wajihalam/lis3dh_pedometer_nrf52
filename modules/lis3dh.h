#ifndef __LIS3DH_H__
#define __LIS3DH_H__

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define  LIS3DH_REGISTER_STATUS_REG_AUX       0x07   // 00000111           r
#define  LIS3DH_REGISTER_OUT_ADC1_L           0x08   // 00001000 output    r
#define  LIS3DH_REGISTER_OUT_ADC1_H           0x09   // 00001001 output    r
#define  LIS3DH_REGISTER_OUT_ADC2_L           0x0A   // 00001010 output    r
#define  LIS3DH_REGISTER_OUT_ADC2_H           0x0B   // 00001011 output    r
#define  LIS3DH_REGISTER_OUT_ADC3_L           0x0C   // 00001100 output    r
#define  LIS3DH_REGISTER_OUT_ADC3_H           0x0D   // 00001101 output    r
#define  LIS3DH_REGISTER_INT_COUNTER_REG      0x0E   // 00001110           r
#define  LIS3DH_REGISTER_WHO_AM_I             0x0F   // 00001111 00110011  r
#define  LIS3DH_REGISTER_TEMP_CFG_REG         0x1F   // 00011111           rw
#define  LIS3DH_REGISTER_CTRL_REG1            0x20   // 00100000 00000111  rw
#define  LIS3DH_REGISTER_CTRL_REG2            0x21   // 00100001 00000000  rw
#define  LIS3DH_REGISTER_CTRL_REG3            0x22   // 00100010 00000000  rw
#define  LIS3DH_REGISTER_CTRL_REG4            0x23   // 00100011 00000000  rw
#define  LIS3DH_REGISTER_CTRL_REG5            0x24   // 00100100 00000000  rw
#define  LIS3DH_REGISTER_CTRL_REG6            0x25   // 00100101 00000000  rw
#define  LIS3DH_REGISTER_REFERENCE            0x26   // 00100110 00000000  rw
#define  LIS3DH_REGISTER_STATUS_REG2          0x27   // 00100111 00000000  r
#define  LIS3DH_REGISTER_OUT_X_L              0x28   // 00101000 output    r
#define  LIS3DH_REGISTER_OUT_X_H              0x29   // 00101001 output    r
#define  LIS3DH_REGISTER_OUT_Y_L              0x2A   // 00101010 output    r
#define  LIS3DH_REGISTER_OUT_Y_H              0x2B   // 00101011 output    r
#define  LIS3DH_REGISTER_OUT_Z_L              0x2C   // 00101100 output    r
#define  LIS3DH_REGISTER_OUT_Z_H              0x2D   // 00101101 output    r
#define  LIS3DH_REGISTER_FIFO_CTRL_REG        0x2E   // 00101110 00000000  rw
#define  LIS3DH_REGISTER_FIFO_SRC_REG         0x2F   // 00101111           r
#define  LIS3DH_REGISTER_INT1_CFG             0x30   // 00110000 00000000  rw
#define  LIS3DH_REGISTER_INT1_SOURCE          0x31   // 00110001 00000000  r
#define  LIS3DH_REGISTER_INT1_THS             0x32   // 00110010 00000000  rw
#define  LIS3DH_REGISTER_INT1_DURATION        0x33   // 00110011 00000000  rw
#define  LIS3DH_REGISTER_CLICK_CFG            0x38   // 00111000 00000000  rw
#define  LIS3DH_REGISTER_CLICK_SRC            0x39   // 00111001 00000000  r
#define  LIS3DH_REGISTER_CLICK_THS            0x3A   // 00111010 00000000  rw
#define  LIS3DH_REGISTER_TIME_LIMIT           0x3B   // 00111011 00000000  rw
#define  LIS3DH_REGISTER_TIME_LATENCY         0x3C   // 00111100 00000000  rw
#define  LIS3DH_REGISTER_TIME_WINDOW          0x3D   // 00111101 00000000  rw

#define  LIS3DH_STATUS_REG_ZYXDA              0x04   // STATUS_REG: XYZ - sample ready
#define  LIS3DH_STATUS_REG_ZYXOR              0x80   // STATUS_REG: XYZ - new set of data has overwritten the previous ones
#define  LIS3DH_CTRL_REG1_XEN                 0x01   // CTRL_REG1: X enable
#define  LIS3DH_CTRL_REG1_YEN                 0x02   // CTRL_REG1: Y enable
#define  LIS3DH_CTRL_REG1_ZEN                 0x04   // CTRL_REG1: Z enable
#define  LIS3DH_CTRL_REG1_XYZEN               0x07   // CTRL_REG1: X+Y+Z enable
#define  LIS3DH_CTRL_REG1_LPEN                0x08   // CTRL_REG1: Low power enable
#define  LIS3DH_CTRL_REG1_DATARATE_POWERDOWN  0x00   // CTRL_REG1: 0000 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_1HZ        0x10   // CTRL_REG1: 0001 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_10HZ       0x20   // CTRL_REG1: 0010 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_25HZ       0x30   // CTRL_REG1: 0011 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_50HZ       0x40   // CTRL_REG1: 0100 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_100HZ      0x50   // CTRL_REG1: 0101 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_200HZ      0x60   // CTRL_REG1: 0110 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_400HZ      0x70   // CTRL_REG1: 0111 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_1500HZ     0x80   // CTRL_REG1: 1000 xxxx
#define  LIS3DH_CTRL_REG1_DATARATE_5000HZ     0x90   // CTRL_REG1: 1001 xxxx
#define  LIS3DH_CTRL_REG4_BLOCKDATAUPDATE     0x80   // CTRL_REG4: 1xxx xxxx
#define  LIS3DH_CTRL_REG4_SCALE_2G            0x00   // CTRL_REG4: xx00 xxxx
#define  LIS3DH_CTRL_REG4_SCALE_4G            0x10   // CTRL_REG4: xx01 xxxx
#define  LIS3DH_CTRL_REG4_SCALE_8G            0x20   // CTRL_REG4: xx10 xxxx
#define  LIS3DH_CTRL_REG4_SCALE_16G           0x30   // CTRL_REG4: xx11 xxxx
#define  LIS3DH_CTRL_REG5_REBOOT              0x80   // CTRL_REG5: 1xxx xxxx
#define  LIS3DH_READ_MULTI_BYTES              0x80


#define LIS3DH_SENSITIVITY_2G  (0.001F)
#define LIS3DH_SENSITIVITY_4G  (0.002F)
#define LIS3DH_SENSITIVITY_8G  (0.004F)
#define LIS3DH_SENSITIVITY_16G (0.012F)

#define LIS3DH_ACC_CONVERT_2G  0.0098F
#define LIS3DH_ACC_CONVERT_4G  0.0049F
#define LIS3DH_ACC_CONVERT_8G  0.00245F
#define LIS3DH_ACC_CONVERT_16G 0.0008167F

#define LIS3DH_I2C_ADDRESS 0x32

void LIS3DH_Init(void);
void LIS3DH_GetAccData(float *acc);

extern bool i2c_write( uint8_t device_address, uint8_t register_address, uint8_t *value, uint8_t number_of_bytes );
extern bool i2c_read( uint8_t device_address, uint8_t register_address, uint8_t *destination, uint8_t number_of_bytes );


#endif /* __LIS3DH_H__ */
