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

#include "box_configurator.h"
#include "hand_location.h"
#include "magnetometer.h"

#define TOTAL_BOXES 9

void poll_hand_function(struct box* box_array);

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
  
  
 
  
  
  int hand_dist;
  int full_dist;
  unsigned long singleSample;
  
  
  
 
  int i;
  int j;
  int k;
  
  int x_pos;
  int y_pos;
  
  int establish_boxes = 0;
  int check_midpoints = 0;
  
  int checking_for_arm = 0;
  unsigned long depth_sum = 0;
  unsigned long depth_avg;
  
  struct box box_array[TOTAL_BOXES];
  build_box_array(box_array);
  
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
    #endif

    // convert the acceleration to a scaled value
    convertUnits(&read_accel, &scaled_accel);   
    
  
  //******************************************************************************//
  //                                 INITIALISATION                               //                                    
  //                                     MODULES                                  //                                            
  //******************************************************************************//
  //PASS TO MAGNETOMETER FUNCTION TO ALLIGN SERVO A PARTICULAR DIRECTION 
  x_pos = align_with_magnetic_field(); 
  // sprintf(buffer, "counter = %d\r\n", counter);
  // SerialOutputString(buffer, &SCI1);
 
  // PASS TO BOX_CONFIGURATOR FUNCITON TO FIND THE BOXES AND THEIR MIDPOINTS
  set_midpoints_box_array(x_pos, 0, box_array);
  
  
  
  //******************************************************************************//
  //                                 WHILE LOOP                                   //                                    
  //                           POLL HAND AND FIND BOX                             //                                            
  //******************************************************************************// 
  
  
  // PASS TO POLL_HAND FUNCTION TO WAIT FOR A HAND
  // NOTE: POLL HAND WILL LOOP INDEFINIETELY WHILST IN A NEUTRAL STATE  
  poll_hand_function(box_array);
    
      
      
   
   
  }
  
}
  

   
   // Use a while loop, scan right 3 boxes, take an average of 30 distance values at each, check if an arm is detected
   // Then check all other boxes, take an average of 15 scans
   // State which box is covered
   // Display info
   // Loop until an average of 30 values at that box is failed
    
    
    
    //_FEED_COP(); /* feeds the dog */
  //} /* loop forever */
  
  /* please make sure that you never leave main */
//}
