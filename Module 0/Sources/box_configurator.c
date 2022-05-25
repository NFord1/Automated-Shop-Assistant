#include <stdio.h>
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include "configurator.h"
#include <stdlib.h>
#include <math.h>

#define TOTAL_BOXES 9
  


void build_box_array() {
  
  int i;
  box box_array[TOTAL_BOXES];
  
  for(i = 0; i < TOTAL_BOXES; i++) {
    box_array[i].box_number = i;    
    
  }
   
}






//CONFIGURATION INTERUPT BUTTON PH0

#define HOME_X
#define HOME_Y
#define HOME_ELEVATION
#define MAX_SERVO_MOVE
#define NUM_DEPTH_TESTS 20
#define UNIT_TO_MM 27
#define PERCENT_ERROR 10
#define WIGGLE_ATTEMPTS 50
#define WIGGLE_SPATIAL_STEP 5

void set_home_pos() {

int servo_home_x;
int servo_home_y;

// MAGNETOMETER TWIST TO HOME

// READ SERVO READING

box_anchor(servo_home_x, servo_home_y);

}


void box_anchor(float home_x, float home_y) {

  
  int 789_azimuth;
  float home_distance;
  float error;
  
  box_array[8].mid_point_x = home_x;
  box_array[8].mid_point_y = home_y;
  
  // TEST DEPTH DIST ANCHOR (BOX 8)
  home_distance = local_average_depth();
  box_array[8].mid_point_depth = home_distance;  
  // CALC AZIMUTH DISTANCE (BOX 7 AND 9)
  789_azimuth = box_789_azimuth_calc(distance);
  // MOVE AZIMUTH DIST (MID-POINT BOX 9)
  setServoPose(home_x + 789_azimuth, home_y);
  test_distance = local_average_depth();
  // COMPARE DEPTH DIST (BOX 8 & 9)
  compare_depth(home_distance, test_distance);
  // TEST DEPTH DIST BOX 7  
  789_azimuth = compare_depth(home_x, home_y, home_distance, 789_azimuth);
  // IF MISSED LD0 ON
  
      //WAIT FOR PH0 TO TEST AGAIN
}

float local_average_depth(){
  float distance[NUM_DEPTH_TESTS];
  float average;
  float sum;
  int i;
  sum = 0;
  average = 0;
  
  
  for(i = 0; i < NUM_DEPTH_TESTS; i++) {
  
  // TEST IF THIS WORKS THIS WAY v
    GetLatestLaserSample(distance[i]);
  }
  
 
  for(i=0;i< NUM_DEPTH_TESTS;i++) 
    {
      sum = sum + distance[i];
    }
   average = sum/NUM_DEPTH_TESTS;
 
 
  return average;  
}


int box_789_azimuth_calc(int anchor_distance) {

  float anchor_dist_mm = UNIT_TO_MM * anchor_distance;

  float theta = atan(150/anchor_dist_mm);
  int azimuth = theta *(MAX_SERVO_MOVE/180)
  return azimuth;
}


void compare_depth(float ref_x, float ref_y, float original, float azimuth){
  int i;
  
  if(test_distance + error >= home_distance && test_distance -error <= home_distance){
  
  // SAMSON TEST BOTH SIDES AT THE SAME TIME IF NOT MISALIGNED  
     
      
    }
  } else{
    return azimuth;
  }
  
}

float misaligned_midpoint(float ref_x, float ref_y, float azimuth){
  float left_test_distance;
  float right_test_distance;
  float left_error; 
  float right_error;
  float wiggle[WIGGLE_ATTEMPTS];
  for(i = 0; i <WIGGLE_ATTEMPTS; i++){
  // MOVE AZIMUTH DIST (BOX 7)
  setServoPose(refx - azimuth, home_y);
  left_test_distance = local_average_depth();
  left_error = test_disance * (PERCENT_ERROR/100 + 1); 
  
  setServoPose(refx - azimuth, home_y);
  right_test_distance = local_average_depth();
  right_error = test_disance * (PERCENT_ERROR/100 + 1);
  
  
  // WIGGLE UNTIL BOTH ARE ALLIGNED
    
  }
  
}
  



void box_mid_point() {
}