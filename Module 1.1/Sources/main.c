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

#define TOTAL_BOXES 9


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
  
  int orientated = 0;
  int counter = 100;
  int toggle = 0;
  int i;
  int j;
  int k;
  
  int x_pos;
  int y_pos;
  
  int establish_boxes = 0;
  
  int checking_for_arm = 0;
  unsigned long depth_sum = 0;
  unsigned long depth_avg;
  
  struct box box_array[TOTAL_BOXES];
  build_box_array(box_array);
  
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
   
   while (orientated == 0) {
   
    if (toggle == 0)
      counter = counter + 10;
    else
      counter = counter - 10;
  
    if (counter > 2000) {
      toggle = 1;
    } else if (counter < 0) {
      toggle = 0;
    }
  
    setServoPose(counter, 0); 
   
    error_code = getRawDataMagnet(&read_magnet);
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
    
      error_code = iicSensorInit();
      printErrorCode(error_code); 
    }
    
    GetLatestLaserSample(&singleSample);
    
      
    sprintf(buffer, "%lu, %d, %d, %d\r\n", singleSample, read_magnet.x, read_magnet.y, read_magnet.z);
    SerialOutputString(buffer, &SCI1);
   
    //Check if facing the correct direction
    //Will have some variance, need to calibrate on the day
                                                   
    magx = 14000;
    magy = 16000;
    magz = -7000;
    
    if (read_magnet.x >= magx - 2000) {
      if (read_magnet.y >= magy - 2000) {
        if (read_magnet.z >= magz - 1000) {
          if (read_magnet.x <= magx +2000) {
            if (read_magnet.y <= magy + 2000) {
              if (read_magnet.z <= magz + 1000) {
                sprintf(buffer, "Orientated!\r\n");
                SerialOutputString(buffer, &SCI1);
                orientated = 1;
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

   }
   
   //sprintf(buffer, "Position %d\r\n", counter);
   //SerialOutputString(buffer, &SCI1);
    // output the data to serial
    
    // Assuming:
    
    // 1 2 3
    // 4 5 6
    // 7 8 9
    
   // Detect midpoints, store values
   
   x_pos = counter;
   
   set_midpoints_box_array(counter, 0, box_array);
   
      
      
      
      
      
      
      
      
      
      
      
      
      /*while (establish_boxes == 0){
        sprintf(buffer, "In whil loop\r\n");
        SerialOutputString(buffer, &SCI1);
        for (i = 0; i < 3; i++) {
          y_pos = i * 300;   
          for (j = -1; j < 2; j++) {
            sprintf(buffer, "j is %d\r\n", j);
            SerialOutputString(buffer, &SCI1);
            x_pos = counter + (j * 300);
            setServoPose(x_pos, y_pos);
            for (k = 0; k < 100; k++) {
              sprintf(buffer, "Reading %d\r\n", k);
              SerialOutputString(buffer, &SCI1);
            }
          }
        }
        establish_boxes = 1;
      }
      */
      
      while (checking_for_arm == 0) {
      
          setServoPose(box_array[9].mid_point_x, box_array[9].mid_point_y);
          depth_sum = 0;
          for (i = 0; i < 200; i++) {
            GetLatestLaserSample(&singleSample);
            depth_sum = depth_sum + singleSample;
            sprintf(buffer, "Depth detected run %d: %lu\r\n", i, singleSample);
            SerialOutputString(buffer, &SCI1);
          }
          depth_avg = depth_sum / 200;
          sprintf(buffer, "Depth at height %d: %lu\r\n", j, depth_avg);
          SerialOutputString(buffer, &SCI1);
         
          setServoPose(box_array[6].mid_point_x, box_array[6].mid_point_y);
          depth_sum = 0;
          for (i = 0; i < 200; i++) {
            GetLatestLaserSample(&singleSample);
            depth_sum = depth_sum + singleSample;
            sprintf(buffer, "Depth detected run %d: %lu\r\n", i, singleSample);
            SerialOutputString(buffer, &SCI1);
          }
          depth_avg = depth_sum / 200;
          sprintf(buffer, "Depth at height %d: %lu\r\n", j, depth_avg);
          SerialOutputString(buffer, &SCI1);
          
          setServoPose(box_array[3].mid_point_x, box_array[3].mid_point_y);
          depth_sum = 0;
          for (i = 0; i < 200; i++) {
            GetLatestLaserSample(&singleSample);
            depth_sum = depth_sum + singleSample;
            sprintf(buffer, "Depth detected run %d: %lu\r\n", i, singleSample);
            SerialOutputString(buffer, &SCI1);
          }
          depth_avg = depth_sum / 200;
          sprintf(buffer, "Depth at height %d: %lu\r\n", j, depth_avg);
          SerialOutputString(buffer, &SCI1);
    }
   
   // Use a while loop, scan right 3 boxes, take an average of 30 distance values at each, check if an arm is detected
   // Then check all other boxes, take an average of 15 scans
   // State which box is covered
   // Display info
   // Loop until an average of 30 values at that box is failed
    
    
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
