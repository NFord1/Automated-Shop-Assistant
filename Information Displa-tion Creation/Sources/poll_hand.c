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

// AFTER INITIALISING THE CODE WILL CONSTANTLY COME BACK HERE
// TO CHECK IF A HAND IS READY TO CHECK A BOX 

void poll_hand_function(struct box* box_array){
  
  char buffer[128];
  unsigned long singleSample;
  float depth_sum = 0;
  float depth_avg;
  int i, j;
  int checking_for_arm = 0;
 
  while (checking_for_arm == 0) {
          
    setServoPose(box_array[9].mid_point_x, box_array[9].mid_point_y);
    lag();
    depth_sum = 0;
          
    for (i = 0; i < 200; i++) {
      GetLatestLaserSample(&singleSample);
      depth_sum = depth_sum + singleSample;
          
      // sprintf(buffer, "Depth detected run %d: %lu\r\n", i, singleSample);
      // SerialOutputString(buffer, &SCI1); 
      }
          
    depth_avg = depth_sum / 200;
                    
    if(depth_avg < box_array[9].mid_point_depth - 2000){
          
      sprintf(buffer, "Depth_Avg = %f\r\n", depth_avg);
      SerialOutputString(buffer, &SCI1);
      sprintf(buffer, "Mid-Depth =  %f\r\n", box_array[9].mid_point_depth);
      SerialOutputString(buffer, &SCI1);
            
      user_selection_box_number(box_array);     
      }
          
          
          
    setServoPose(box_array[6].mid_point_x, box_array[6].mid_point_y);
    lag();
    depth_sum = 0;
          
    for (i = 0; i < 200; i++) {
      GetLatestLaserSample(&singleSample);
      depth_sum = depth_sum + singleSample;             
      }

    depth_avg = depth_sum / 200;

    if(depth_avg < box_array[6].mid_point_depth - 2000){
      user_selection_box_number(box_array);                 
      }
         
          
    setServoPose(box_array[3].mid_point_x, box_array[3].mid_point_y);
    lag();
    depth_sum = 0;
          
    for (i = 0; i < 200; i++) {
      GetLatestLaserSample(&singleSample);
      depth_sum = depth_sum + singleSample;   
      }
          
    depth_avg = depth_sum / 200;
          
          
    if(depth_avg < box_array[3].mid_point_depth - 2000){
      user_selection_box_number(box_array);
     } 
   }
}