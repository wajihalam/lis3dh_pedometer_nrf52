
#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "lis3dh_acc_driver.h"



#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


// Defining variables
int flag = 0;
int flag1 = 0;
int16_t x_val, y_val, z_val;
float faccel_Y;
int i,a,count = 0;
float paccel_Y = 0.0f;
float sampled_Y;
float sample[50];
int acc_val = 0;


/*
* Function to calculate the step count from accelerometer data
*/

void calculate()
{
  lis3dh_ReadAcc(&x_val,&y_val,&z_val);
  for(i = 0; i < 50; i++)
    {
     faccel_Y = (paccel_Y+y_val)/2;
     paccel_Y = y_val;
     sample[i] = faccel_Y;
     sampled_Y += sample[i];
     NRF_LOG_INFO("Sampled value inside for loop is: %d", sampled_Y);
    }
    sampled_Y = sampled_Y/50;
    NRF_LOG_INFO("The average Sampled value is: %d", sampled_Y);
    if ((sampled_Y <= -4000 && flag == 0) || (sampled_Y >= 4000 && flag1 == 0))
    {
      NRF_LOG_INFO("Sampled value before count is: %d", sampled_Y);
      count += 1;
      flag = 1;
      flag1 = 1;
    }
    else if ((sampled_Y <= -4000 && flag == 0) || (sampled_Y >= 4000 && flag1 == 0))
    {
      a = a+1;
    }
    if (sampled_Y >= -4000)
    {
      flag  = 0;
    }
    else if (sampled_Y <= 4000)
    {
      flag1 = 0;
    }
}


/**
 * @brief Function for main application entry.
 */
int main(void)
{
    ret_code_t err_code;
    uint8_t address;
    uint8_t sample_data;
    bool detected_device = false;

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("Accelerometer scanner started.");
    NRF_LOG_FLUSH();
    

    // create arrays which will hold x,y & z co-ordinates values of acc and gyro
    static int16_t AccValue[3];

    //bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS); // initialize the leds and buttons
    twi_init();

    
    nrf_delay_ms(1000); // give some delay

    lis3dh_verify_product_id();

    while(lis3dh_init() == false) // wait until lis3dh sensor is successfully initialized
    {
      NRF_LOG_INFO("LIS3DH initialization failed!!!"); // if it failed to initialize then print a message
      nrf_delay_ms(1000);
    }

   NRF_LOG_INFO("LIS3DH Init Successfully!!!"); 

   NRF_LOG_INFO("Reading Values from ACC"); // display a message to let the user know that the device is starting to read the values
   nrf_delay_ms(2000);

    
    while (true)
    {
        err_code = nrf_drv_twi_rx(&m_twi, LIS3DH_ADDRESS, &sample_data, sizeof(sample_data));
        if (err_code == NRF_SUCCESS)
        {
            detected_device = true;
            NRF_LOG_INFO("TWI device detected at address 0x%x.", LIS3DH_ADDRESS);
        }
        NRF_LOG_FLUSH();
        /* Empty loop. */

        if(lis3dh_ReadAcc(&AccValue[0], &AccValue[1], &AccValue[2]) == true) // Read acc value from mpu6050 internal registers and save them in the array
        {
          NRF_LOG_INFO("ACC Values:  x = %d  y = %d  z = %d", AccValue[0], AccValue[1], AccValue[2]); // display the read values
        }
        else
        {
          NRF_LOG_INFO("Reading ACC values Failed!!!"); // if reading was unsuccessful then let the user know about it
        }

       nrf_delay_ms(1000); // give some delay 

       calculate();
       acc_val = count;
       NRF_LOG_INFO("The counted value is: %d", acc_val);
     
    }
}

/** @} */
