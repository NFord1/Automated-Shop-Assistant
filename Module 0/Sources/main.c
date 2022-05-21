#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>

#include "pll.h"
#include "simple_serial.h"

#include "l3g4200d.h"

#include "servo.h"
#include "laser.h"

#include "gyro.h"


void printErrorCode(IIC_ERRORS error_code) {
  char buffer[128];  
  switch (error_code) {
    case NO_ERROR: 
      sprintf(buffer, "IIC: No error\r\n");
      break;
    
    case NO_RESPONSE: 
      sprintf(buffer, "IIC: No response\r\n");
      break;
    
    case NAK_RESPONSE:
      sprintf(buffer, "IIC: No acknowledge\r\n");
      break;
    
    case IIB_CLEAR_TIMEOUT:
      sprintf(buffer, "IIC: Timeout waiting for reply\r\n");
      break;
    
    case IIB_SET_TIMEOUT: 
      sprintf(buffer, "IIC: Timeout not set\r\n");
      break;
    
    case RECEIVE_TIMEOUT:
      sprintf(buffer, "IIC: Received timeout\r\n");
      break;
    
    case IIC_DATA_SIZE_TOO_SMALL:
      sprintf(buffer, "IIC: Data size incorrect\r\n");
      break;

    default:
      sprintf(buffer, "IIC: Unknown error\r\n");
      break;
  }
    
  SerialOutputString(buffer, &SCI1);
}

void main(void) {

  AccelRaw read_accel;
  AccelScaled scaled_accel;

  GyroRaw read_gyro;
  MagRaw read_magnet;
  
  IIC_ERRORS error_code = NO_ERROR;
  
  char buffer[128];  
  
  unsigned long singleSample;
  
  int magx;
  int magy;
  int magz;
  
  int hand_dist;
  int full_dist;
  
  //assert(error_code != NO_ERROR);

  #ifndef SIMULATION_TESTING

  // make sure the board is set to 24MHz
  //  this is needed only when not using the debugger
  PLL_Init();

  // initialise PWM
  PWMinitialise();
  setServoPose(100, 0);

  #endif
  
  // initialise the simple serial
  SerialInitialise(BAUD_9600, &SCI1);
  
  #ifndef SIMULATION_TESTING
  
  // initialise the sensor suite
  error_code = iicSensorInit();
  
  // write the result of the sensor initialisation to the serial
  if (error_code == NO_ERROR) {
    sprintf(buffer, "NO_ERROR\r\n");
    SerialOutputString(buffer, &SCI1);
  } else {
    sprintf(buffer, "ERROR %d\r\n");
    SerialOutputString(buffer, &SCI1);
  }

  laserInit();
  
  #else
  
  #endif

  Init_TC6();
  
	EnableInterrupts;
  //COPCTL = 7;
  _DISABLE_COP();
    
  for(;;) {
  
    #ifndef SIMULATION_TESTING
  
    // read the raw values
    error_code = getRawDataGyro(&read_gyro);   
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
       
      error_code = iicSensorInit();
      printErrorCode(error_code);   
    }
    
    error_code = getRawDataAccel(&read_accel);
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
    
      error_code = iicSensorInit();
      printErrorCode(error_code); 
    }
    
    error_code = getRawDataMagnet(&read_magnet);
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
    
      error_code = iicSensorInit();
      printErrorCode(error_code); 
    }
    
    GetLatestLaserSample(&singleSample);
        
    #else
    
    // inject some values for simulation
    read_gyro.x = 123; read_gyro.y = 313; read_gyro.z = 1002;
    read_accel.x = 124; read_accel.y = 312; read_accel.z = 2002;
    //read_magnet.x = 125; read_magnet.y = 311; read_magnet.z = 3002;
    
    #endif

    // convert the acceleration to a scaled value
    convertUnits(&read_accel, &scaled_accel);   
    
    // format the string of the sensor data to go the the serial 
   
      
    sprintf(buffer, "%lu, %d, %d, %d\r\n", singleSample, read_magnet.x, read_magnet.y, read_magnet.z);
   SerialOutputString(buffer, &SCI1);
   
   //Check if facing the correct direction
   //Will have some variance, need to calibrate on the day
   
   magx = 0;
   magy = 0;
   magz = 0;
    
   if (read_magnet.x >= magx - 200) {
     if (read_magnet.y >= magy - 200) {
       if (read_magnet.z >= magz - 200) {
         if (read_magnet.x <= magx +200) {
          if (read_magnet.y <= magy + 200) {
            if (read_magnet.z <= magz + 200) {
              sprintf(buffer, "Orientated!\r\n");
              SerialOutputString(buffer, &SCI1);
            }
          }
         }
       }
     }
   }
   
   hand_dist = 0;
   full_dist = 0;
   
   //Test if empty, full or a hand is present
   
   if (singleSample <= hand_dist) {
      sprintf(buffer, "Hand!\r\n");
      SerialOutputString(buffer, &SCI1);
   } else if (singleSample <= full_dist) {
      sprintf(buffer, "Full!\r\n");
      SerialOutputString(buffer, &SCI1);
   } else {
      sprintf(buffer, "Empty!\r\n");
      SerialOutputString(buffer, &SCI1);
   }

    
    // output the data to serial
    
    
    
    
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
