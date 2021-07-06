# Pedometer for NRF52 Devices
This repo contains code to use lis3dh as pedometer for nrf52 devices


# Working of the Pedometer
This code implements a pedometer using data (X,Y,Z) collected from accelerometer in real-time. The code is written in C. The algorithm works as follows:

  1. Filtering: The data from the accelerometer contains noise. Hence, there is a need for filtering to smooth out the signals and reduce errors. 
     The averaging Filter is used in this case which uses a low-pass filter. The Y-axis value is used for this. 
  2. Average collected data: The averaged data is collected for 50 samples each time. The collected samples are then averaged and then compared with the two extreme point of
     wrist movement to calculate the step count. Two flags are used to remove false step detection. For example: if a person is in the same position but he moves his hand. 
     
     
# Porting to other accelerometer
This code can be ported to other accelerometer as well. You will need to modify lis3dh_acc_driver.c and lis3dh_acc_driver.h file according to the datasheet of the selected
microcontroller. 


# Using it in Segger Embedded Studio
You will need to put this directory inside nrf5 sdk in the following location: \nRF5_SDK_17.0.2_d674dde\examples\myproject\lis3dh_nrf52


# References
http://www.analog.com/en/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html
