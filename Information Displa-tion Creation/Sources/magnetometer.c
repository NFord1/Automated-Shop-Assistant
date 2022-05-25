#include <hidef.h>      
#include <assert.h>
#include "derivative.h"      
#include <stdio.h>
#include "pll.h"
#include "simple_serial.h"
#include "l3g4200d.h"
#include "servo.h"
#include "gyro.h"

#include <stdio.h>
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include "box_configurator.h"
#include <stdlib.h>

#define MAGX 14000
#define MAGY 26000
#define MAGZ -2000


// THIS FUNCTIONS PURPOSE TO ALLIGN THE SERVO TO ALWAYS FACE
// THE SAME POLAR CO-ORDINATES IN CASE OF BEING MOVED OR ADJUSTED

int align_with_magnetic_field(){
  // INITILISATION VARIABLES
  int counter = 100;
  int magx;
  int magy;
  int magz;
  char buffer[128];
  int orientated = 0;
  int toggle = 0;
  unsigned long singleSample;
  MagRaw read_magnet;
   
  // TEST IF ORIENTED ALREADY  
  while (orientated == 0) {
    // MOVE AROUND UNTIL ORIENTED
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
   
    /*error_code = getRawDataMagnet(&read_magnet);
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
    
      error_code = iicSensorInit();
      printErrorCode(error_code); 
    }*/
    
    GetLatestLaserSample(&singleSample);
    
      
    sprintf(buffer, "Please wait, while orientating...\r\n");
    SerialOutputString(buffer, &SCI1);
   
    //Check if facing the correct direction
    //Will have some variance, need to calibrate on the day
    
    
    // PREDEFINED DEFAULTED MAGNETOMETER READINGS
                                                   
    magx = MAGX;
    magy = MAGY;
    magz = MAGZ;
    
    // IF MAGNETOMETER FALLS WITHIN AN ERROR RANGE
    
    if (read_magnet.x >= magx - 2000) {
      if (read_magnet.y >= magy - 2000) {
        if (read_magnet.z >= magz - 1000) {
          if (read_magnet.x <= magx +2000) {
            if (read_magnet.y <= magy + 2000) {
              if (read_magnet.z <= magz + 1000) {
                sprintf(buffer, "Orientated!\r\n");
                SerialOutputString(buffer, &SCI1);
                // IT IS ORIENTED
                orientated = 1;
              }
            }
          }
        }
      }
    }
  }
 
 // RETURN SERVO POSITION   
 return counter;
    
}