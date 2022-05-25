#include <stdio.h>
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include "box_configurator.h"
#include <stdlib.h>
#include <math.h>

#define TOTAL_BOXES 9
  


void build_box_array(struct box box_array[TOTAL_BOXES]) {
  
  int i;
  
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

float servo_home_x;
float servo_home_y;
struct box box_array[TOTAL_BOXES];

// MAGNETOMETER TWIST TO HOME

// READ SERVO READING

set_midpoints_box_array(servo_home_x, servo_home_y, box_array);

}


void set_midpoints_box_array(float home_x, float home_y, struct box* box_array) {
  

  
  float azimuth_789;
  float azimuth_456;
  float azimuth_123;
  float elevation;
  float depth_distance;
  
  
  
  // TEST DEPTH DIST ANCHOR (BOX 8)
  setServoPose(home_x, home_y);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(8, home_x, home_y, depth_distance); 
  // CALC AZIMUTH DISTANCE (BOX 7 AND 9)
  azimuth_789 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 7 and 9)
  misaligned_midpoint(7,9,home_x, home_y, azimuth_789);
  
  
  // CALC ELEVATION (TO MOVE BOX 5)
  elevation = azimuth_calc(box_array[8].mid_point_depth, 1);
  // TEST DEPTH DISTANCE (BOX 5)
  setServoPose(home_x, home_y + elevation);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(5, home_x, home_y, depth_distance);
  // CALC AZIMUTH DISTANCE (BOX 4 AND 6)
  azimuth_456 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 4 AND 6)
  misaligned_midpoint(4,6,home_x, home_y + elevation, azimuth_456);
  
  
  
  // CALC ELEVATION (TO MOVE BOX 2)
  elevation = azimuth_calc(box_array[8].mid_point_depth, 2);
  // TEST DEPTH DISTANCE (BOX 2)
  setServoPose(home_x, home_y + elevation);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(2, home_x, home_y, depth_distance);
  // CALC AZIMUTH DISTANCE (BOX 1 AND 3)
  azimuth_123 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 1 AND 3)
  misaligned_midpoint(1,3,home_x, home_y + elevation, azimuth_123);

}

float local_average_depth(){
  unsigned long distance[NUM_DEPTH_TESTS];
  float average;
  float sum;
  int i;
  sum = 0;
  average = 0;
  
  
  for(i = 0; i < NUM_DEPTH_TESTS; i++) {
  
  // TEST IF THIS WORKS THIS WAY v
    GetLatestLaserSample(distance);
  }
  
 
  for(i=0;i< NUM_DEPTH_TESTS;i++) 
    {
      sum = sum + (float)distance[i];
    }
   average = sum/NUM_DEPTH_TESTS;
 
 
  return average;  
}


float azimuth_calc(float anchor_distance, int scale) {

  float anchor_dist_mm;
  float theta;
  float azimuth;

  anchor_dist_mm = UNIT_TO_MM * (1/anchor_distance);
  theta = (float) atan((double)(scale *150)/(double)anchor_dist_mm);
  azimuth = (float) theta *(MAX_SERVO_MOVE/180);
  
  return azimuth;
}


void misaligned_midpoint(int left_box_num, int right_box_num, float ref_x, float ref_y, float azimuth){
  int i,j;
  
  float ref_distance
  float left_test_distance;
  float right_test_distance;
  float ref_error;
  float left_error; 
  float right_error;
  
  float pos_neg_azimuth[2] = {-1, 1};
  float adj_azimuth;
  float wiggle[WIGGLE_ATTEMPTS];
  
  setServoPose(ref_x, ref_y);
  ref_distance = local_average_depth();
  
  // WIGGLE UNTIL BOTH ARE ALLIGNED
  for (i =0; i < 2; i++){
   
   for(j = 0; j <WIGGLE_ATTEMPTS; i++){
   
    adj_azimuth = (pos_neg_azimuth[i] * j * WIGGLE_SPATIAL_STEP) + azimuth; 
 
    // MOVE AZIMUTH DIST (BOX LEFT/ NEG DIRECTION)
    setServoPose(ref_x - adj_azimuth, ref_y);
    left_test_distance = local_average_depth();
    left_error = left_test_disance * (PERCENT_ERROR/100); 
  
    // MOVE AZIMUTH DIST (BOX RIGHT/ POS DIRECTION)
    setServoPose(ref_x + adj_azimuth, ref_y);
    right_test_distance = local_average_depth();
    right_error = right_test_disance * (PERCENT_ERROR/100);
  
    if (left_test_distance - left_error < ref_distance && left_test_distance + left_error > ref_distance){
      // IF LEFT SIDE FALLS WITHIN ERROR BOUNDS
      if (right_test_distance - right_error < ref_distance && right_test_distance + right_error > ref_distance){  
        // IF RIGHT SIDE FALLS WITHIN ERROR BOUNDS
        add_midpoint_to_struct(left_box_num, ref_x - adj_azimuth , ref_y, left_test_distance);
        add_midpoint_to_struct(right_box_num, ref_x + adj_azimuth , ref_y, right_test_distance);
        void adj_azimuth;
        
        }
      }
    }
  }
  
  // ******* FAILED ALLIGNMENT, DO WE WANT TO ADD CONDITION? ********* //
  //                                                                   //
  //                                                                   //
  //                                                                   //
  //                                                                   //
  //                                                                   //

}                                                                      

void add_midpoint_to_struct(int box_num, float midpoint_x, float midpoint_y, float depth){
  
  box_array[box_num].midpoint_x = midpoint_x;
  box_array[box_num].midpoint_y = midpoint_y;
  box_array[box_num].mid_point_depth = depth;   
  
}






}