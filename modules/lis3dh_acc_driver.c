#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "lis3dh_acc_driver.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


//Initializing TWI0 instance
#define TWI_INSTANCE_ID     0

// A flag to indicate the transfer state
static volatile bool m_xfer_done = false;


__STATIC_INLINE void data_handler(uint8_t temp) {
  NRF_LOG_INFO("Data Received on i2c: 0x%02x.", temp);
}


//Event Handler
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    //Check the event to see what type of event occurred
    switch (p_event->type)
    {
        //If data transmission or receiving is finished
	case NRF_DRV_TWI_EVT_DONE:
        m_xfer_done = true;//Set the flag
        break;
        
        default:
        // do nothing
          break;
    }
}


/*******************************************************************************
* Function Name		: twi_init
* Description		: Initializing TWI interface				
* Return		: None
*******************************************************************************/

void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = 16,
       .sda                = 14,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}



/*******************************************************************************
* Function Name		: lis3dh_register_write
* Description		: Generic Writing function. It must be fullfilled with I2C reading functions.					
* Input			: Register Address
* Output		: Data REad
* Return		: None
*******************************************************************************/

bool lis3dh_register_write(uint8_t register_address, uint8_t value)
{
    ret_code_t err_code;
    uint8_t tx_buf[LIS3DH_ADDRESS_LEN+1];
	
    //Write the register address and data into transmit buffer
    tx_buf[0] = register_address;
    tx_buf[1] = value;

    //Set the flag to false to show the transmission is not yet completed
    m_xfer_done = false;
    
    //Transmit the data over TWI Bus
    err_code = nrf_drv_twi_tx(&m_twi, LIS3DH_ADDRESS, tx_buf, LIS3DH_ADDRESS_LEN+1, false);
    
    //Wait until the transmission of the data is finished
    while (m_xfer_done == false)
    {
      }

    // if there is no error then return true else return false
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
    
    return true;	
}

/*******************************************************************************
* Function Name		: lis3dh_register_read
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions. In this case, I2C.					
* Input			: Register Address
* Output		: Data REad
* Return		: None
*******************************************************************************/
/*
  A Function to read data from the Lis3dh
*/ 
bool lis3dh_register_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
    ret_code_t err_code;

    //Set the flag to false to show the receiving is not yet completed
    m_xfer_done = false;
    
    // Send the Register address where we want to write the data
    err_code = nrf_drv_twi_tx(&m_twi, LIS3DH_ADDRESS, &register_address, 1, false);
	  
    //Wait for the transmission to get completed
    while (m_xfer_done == false){}
    
    // If transmission was not successful, exit the function with false as return value
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }

    //set the flag again so that we can read data from the MPU6050's internal register
    m_xfer_done = false;
	  
    // Receive the data from the MPU6050
    err_code = nrf_drv_twi_rx(&m_twi, LIS3DH_ADDRESS, destination, number_of_bytes);
		
    //wait until the transmission is completed
    while (m_xfer_done == false){}
	
    // if data was successfully read, return true else return false
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
    
    return true;
}

/*
  Function to verify communication with lis3dh
*/ 

bool lis3dh_verify_product_id(void)
{
    uint8_t who_am_i; // create a variable to hold the who am i value


    // Note: All the register addresses including WHO_AM_I are declared in 
    // lis3dh.h file, you can check these addresses and values from the
    // datasheet of your slave device.
    if (lis3dh_register_read(LIS3DH_WHO_AM_I, &who_am_i, 1))
    {
        if (who_am_i != 0x33)
        {
            NRF_LOG_INFO("Register read who am I not working");
            return false;
            
        }
        else
        {
            NRF_LOG_INFO("REgister read who am I is working");
            return true;
        }
    }
    else
    {
    NRF_LOG_INFO("REgister read who am I issssssssssssssss working");
        return false;
    }
}


/*
  Function to initialize the lis3dh
*/ 
bool lis3dh_init(void)
{   
  bool transfer_succeeded = true;
	
  //Check the id to confirm that we are communicating with the right device
  transfer_succeeded &= lis3dh_verify_product_id();
	
  if(lis3dh_verify_product_id() == false)
    {
	return false;
      }

  // Set the registers with the required values, see the datasheet to get a good idea of these values
  (void)lis3dh_register_write(LIS3DH_CTRL_REG1 , CTRL1); 
  (void)lis3dh_register_write(LIS3DH_CTRL_REG2 , CTRL2); 
  (void)lis3dh_register_write(LIS3DH_CTRL_REG3 , CTRL3); 						
  (void)lis3dh_register_write(LIS3DH_CTRL_REG4 , CTRL4); 
  (void)lis3dh_register_write(LIS3DH_CTRL_REG5 , CTRL5); 
  (void)lis3dh_register_write(LIS3DH_CTRL_REG6 , CTRL6);   
  (void)lis3dh_register_write(INT1_THS  ,   THS);
  (void)lis3dh_register_write(INT1_DURATION  ,   DURATION);
  (void)lis3dh_register_write(INT1_CFG  ,   CFG);

  return transfer_succeeded;
}


/*
  A Function to read accelerometer's values from the internal registers of Lis3dh
*/ 
bool lis3dh_ReadAcc( int16_t *pACC_X , int16_t *pACC_Y , int16_t *pACC_Z )
{
  uint8_t tmp[6] = {};
  int16_t tmp_int16[3] = {0};
  uint8_t status = 0;
  bool ret = false;		
  
  if(lis3dh_register_read((LIS3DH_OUT_X_L | LIS3DH_READ_MULTI_BYTES), tmp, 6) == true)
  {
    
    lis3dh_register_read((LIS3DH_OUT_X_L | LIS3DH_READ_MULTI_BYTES), tmp, 6);

   *pACC_X = (int16_t)((tmp[1]<< 8) | tmp[0]);
   *pACC_Y = (int16_t)((tmp[3]<< 8) | tmp[2]);
   *pACC_Z = (int16_t)((tmp[5]<< 8) | tmp[4]);
		
    ret = true;
    }
  
  return ret;
}