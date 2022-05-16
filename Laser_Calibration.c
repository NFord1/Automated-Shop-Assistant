#include <stdio.h>
#include "laser.h"
#include "simple_serial.h"
#include "derivative.h"      /* derivative-specific definitions */
#include "Laser_Calibration.h"

// Using code modules from stewarts week 9 major project drivers
// Scale data
// align with calibration box

void laserCalibration(unsigned long *sample){

  char buffer[128];
  
  //Intended distance
  int dist=1;

  //Check if the distance to calibration box is correct
  if (sample > dist){
    //move foreward
    sprintf(buffer, "Move Device Foreward\r\n");
  
    SerialOutputString(buffer, &SCI1);
  
  } else if (sample < dist){
    //move back
    sprintf(buffer, "Move Device Back\r\n");
  
    SerialOutputString(buffer, &SCI1);
  
  } else if (sample == dist){
    //correct position
    sprintf(buffer, "Calibrated: Correct Position\r\n");
  
    SerialOutputString(buffer, &SCI1);
  }
  
}

// Could use accelerometer or gyro to warn if moving wrong way 
// if incorporation of another sensor is required
