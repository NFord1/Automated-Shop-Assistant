#include <stdio.h>
#include <hidef.h>
#include <assert.h>
#include "derivative.h"
#include "pll.h" 
#include "l3g4200d.h"
#include "gyro.h"
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include "box_configurator.h"
#include <stdlib.h>
#include <math.h>
#include "simple_serial.h"

#define TOTAL_BOXES 9

char buffer[128]; 
  


void build_box_array(struct box box_array[TOTAL_BOXES]) {
  
  int i;
  
  for(i = 0; i < TOTAL_BOXES; i++) {
    box_array[i].box_number = i;    
    
  }
  /*
  box_array[1]->item = "Apples";
  box_array[2]->item = "Oat Bar";
  box_array[3]->item = "Water Bottle";
  box_array[4]->item = "Frozen Pizza";
  box_array[5]->item = "Bread Roll";
  box_array[6]->item = "Cake";
  box_array[7]->item = "Chocolate";
  box_array[8]->item = "Mince Beef";
  box_array[9]->item = "Milk";
  
  box_array[1].price = 0.5;
  box_array[2].price = 1;
  box_array[3].price = 1;
  box_array[4].price = 5;
  box_array[5].price = 2;
  box_array[6].price = 10;
  box_array[7].price = 4;
  box_array[8].price = 12;
  box_array[9].price = 3.5;
  
  box_array[1].kj_per = 200;
  box_array[2].kj_per = 800;
  box_array[3].kj_per = 0;
  box_array[4].kj_per = 4000;
  box_array[5].kj_per = 1600;
  box_array[6].kj_per = 10200;
  box_array[7].kj_per = 3500;
  box_array[8].kj_per = 6000;
  box_array[9].kj_per = 2400;
  
  box_array[1].location = "Queensland";
  box_array[2].location = "South Australia";
  box_array[3].location = "Blue Mountains";
  box_array[4].location = "Italy";
  box_array[5].location = "Made Locally";
  box_array[6].location = "Western Australia";
  box_array[7].location = "Sydney";
  box_array[8].location = "Melbourne";
  box_array[9].location = "Queensland";
  
  box_array[1].allergy = "None";
  box_array[2].allergy = "Nuts, Gluten, Dairy, Egg";
  box_array[3].allergy = "None";
  box_array[4].allergy = "Gluten, Dairy, Egg";
  box_array[5].allergy = "Gluten";
  box_array[6].allergy = "Gluten, Dairy, Egg";
  box_array[7].allergy = "Gluten, Dairy, Egg";
  box_array[8].allergy = "Meat";
  box_array[9].allergy = "Dairy";
  
  */
    
}


void lag() {
  
  int i;
  int j = 1;
  int k;
  for(i = 0; i<9600; i++) {
    for(k = 0; k< 100; k++) {
      
      j = i + 1;
    }
  }
}



//CONFIGURATION INTERUPT BUTTON PH0

//#define HOME_X
//#define HOME_Y
//#define HOME_ELEVATION
#define MAX_SERVO_MOVE 2600
#define NUM_DEPTH_TESTS 20
#define UNIT_TO_MM 27
#define WIGGLE_ATTEMPTS 50
#define WIGGLE_SPATIAL_STEP 5

void set_midpoints_box_array(float home_x, float home_y, struct box* box_array) {
  

  
  float azimuth_789;
  float azimuth_456;
  float azimuth_123;
  float elevation;
  float depth_distance;
  
  sprintf(buffer, "home_x = %f\r\n", home_x);
  SerialOutputString(buffer, &SCI1);
  
  // TEST DEPTH DIST ANCHOR (BOX 8)
  setServoPose(home_x, home_y);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(box_array, 8, home_x, home_y, depth_distance); 
  // CALC AZIMUTH DISTANCE (BOX 7 AND 9)
  azimuth_789 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 7 and 9)
  misaligned_midpoint(box_array, 7,9,home_x, home_y, azimuth_789);
  
  
  // CALC ELEVATION (TO MOVE BOX 5)
  elevation = azimuth_calc(box_array[8].mid_point_depth, 1);
  // TEST DEPTH DISTANCE (BOX 5)
  setServoPose(home_x, home_y + elevation);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(box_array, 5, home_x, home_y + elevation, depth_distance);
  // CALC AZIMUTH DISTANCE (BOX 4 AND 6)
  azimuth_456 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 4 AND 6)
  misaligned_midpoint(box_array, 4,6,home_x, home_y + elevation, azimuth_456);
  
  
  
  // CALC ELEVATION (TO MOVE BOX 2)
  elevation = azimuth_calc(box_array[8].mid_point_depth, 2);
  // TEST DEPTH DISTANCE (BOX 2)
  setServoPose(home_x, home_y + elevation);
  depth_distance = local_average_depth();
  add_midpoint_to_struct(box_array, 2, home_x, home_y + elevation, depth_distance);
  // CALC AZIMUTH DISTANCE (BOX 1 AND 3)
  azimuth_123 = azimuth_calc(depth_distance, 1);
  // SET MIDPOINT (BOX 1 AND 3)
  misaligned_midpoint(box_array, 1,3,home_x, home_y + elevation, azimuth_123);

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
    GetLatestLaserSample(&distance[i]);
  }
  
 
  for(i=0;i< NUM_DEPTH_TESTS;i++) 
    {
      sum = sum + (float)distance[i];
    }
   average = sum/NUM_DEPTH_TESTS;
 
 
  return average;  
}


float azimuth_calc(float anchor_distance, int scale) { 

  double anchor_dist_mm;
  float theta;
  float azimuth;
  double double_var;
  double double_theta;
  float one_servo_degree;
  
  PWMinitialise();

  anchor_dist_mm = (double) anchor_distance * 0.04;
  double_var = (double)(scale) * (150/anchor_dist_mm);
  double_theta = atan(double_var);
  theta = (float) double_theta;
  azimuth = theta * 828.025 * 1.5;
  
  sprintf(buffer, "Azimuth = %f\r\n", azimuth);
  SerialOutputString(buffer, &SCI1);
  
  return azimuth;
}


int misaligned_midpoint(struct box *box_array, int left_box_num, int right_box_num, float ref_x, float ref_y, float azimuth){
  int i,j;
  
  float ref_distance;
  float left_test_distance;
  float right_test_distance;
  float left_error; 
  float right_error;
  
  float pos_neg_azimuth[2] = {-1, 1};
  float adj_azimuth;
  
  PWMinitialise();
  
  setServoPose(ref_x, ref_y);
  lag();
  ref_distance = box_array[8].mid_point_depth;
  
  // WIGGLE UNTIL BOTH ARE ALLIGNED
  for (i =0; i < 2; i++){
   
   for(j = 0; j <WIGGLE_ATTEMPTS; i++){
   
    adj_azimuth = (pos_neg_azimuth[i] * j * WIGGLE_SPATIAL_STEP) + azimuth; 
 
    // MOVE AZIMUTH DIST (BOX LEFT/ NEG DIRECTION)
    setServoPose(ref_x - adj_azimuth, ref_y);
    lag();
    left_test_distance = local_average_depth();
    left_error = left_test_distance * 0.2; 
  
    // MOVE AZIMUTH DIST (BOX RIGHT/ POS DIRECTION)
    setServoPose(ref_x + adj_azimuth, ref_y);
    lag();
    right_test_distance = local_average_depth();
    right_error = right_test_distance * 0.2;
    sprintf(buffer, "%f - %f < %f\r\n", left_test_distance, left_error, ref_distance);
    SerialOutputString(buffer, &SCI1);
    if (left_test_distance - left_error < ref_distance && left_test_distance + left_error > ref_distance){
      // IF LEFT SIDE FALLS WITHIN ERROR BOUNDS
      if (right_test_distance - right_error < ref_distance && right_test_distance + right_error > ref_distance){  
        // IF RIGHT SIDE FALLS WITHIN ERROR BOUNDS
        add_midpoint_to_struct(box_array, left_box_num, ref_x - adj_azimuth , ref_y, left_test_distance);
        add_midpoint_to_struct(box_array, right_box_num, ref_x + adj_azimuth , ref_y, right_test_distance);
        return 0;
               
        }
      }
    }
  } 

}                                                                      

void add_midpoint_to_struct(struct box *box_array, int box_num, float midpoint_x, float midpoint_y, float depth){
  
  box_array[box_num].mid_point_x = midpoint_x;
  box_array[box_num].mid_point_y = midpoint_y;
  box_array[box_num].mid_point_depth = depth;   
  
}

