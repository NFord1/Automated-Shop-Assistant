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

#define TOTAL_BOXES 9

// After hand has been identified, code needs to come here
// to identify which box the user is convering







int user_selection_box_number(struct box* box_array){
// This function is the main function and returns
// the identity of the box number that the user is
// trying to get information from
// 
//   ___________
//  |   |   |   |
//  | 1 | 2 | 3 |
//  |---|---|---|
//  |   |   |   |   <--- Hand goes this way
//  | 4 | 5 | 6 |
//  |---|---|---|
//  | 7 | 8 | 9 |
//  |___|___|___| 
  
  int box_number;
    
  hand_convered_boxes(box_array);
  box_number = highest_box_probability(box_array);
  
  
  // print info regarding whats in the box
  return box_number;

  
}








void hand_convered_boxes(struct box* box_array){
// This function identifies which of the boxes have been
// convered by the users hand   
  
  int i;
  float distance;
  float hand_trigger = 600;
  
  for(i = 0; i < TOTAL_BOXES; i++){
   
   // Based on pre-defined mid-point (x,y) move servo to sample the 
   // distance away of the object
   setServoPose(box_array[i].mid_point_x, box_array[i].mid_point_y);
   local_average_depth();
   
   // If the closest distance is closer than the pre-definied target
   // hand is presentt
   
   if (distance < box_array[i].mid_point_depth - hand_trigger){
    
    box_array[i].hand_covering = 1;
   } 
   else{
    
   box_array[i].hand_covering = 0; 
   }
    
  } 
}


int highest_box_probability(struct box* box_array){
// This function takes all of the boxes that are covered
// and calculates which one is the most likely to be the
// one selected by the user  
//
//  Example:
//   ___________
//  |   |   |   |
//  |{1}| 2 | 3 |
//  |---|---|---|
//  |   |   |   |
//  | 4 |{5}|{6}|    <-- This is most likely to be 5 
//  |---|---|---|        Because as the user has there arm out
//  | 7 | 8 |{9}|        it covers 6 and 5
//  |___|___|___|        i.e. 1 and 9 are false readings
  
  int i;
  int k = 0;
  
  int highest_priority[3];
  int max;
  char buffer[128]; 
  
  if (box_array[3].hand_covering == 1) {
   highest_priority[0] = 1;
  
   if (box_array[2].hand_covering == 1){
    highest_priority[0] = 2;
   
    if (box_array[2].hand_covering == 1){
     highest_priority[0] = 3;
    
    } 
   }    
  }

  if (box_array[6].hand_covering == 1) {
   highest_priority[1] = 1;
  
   if (box_array[5].hand_covering == 1){
    highest_priority[1] = 2;
   
    if (box_array[4].hand_covering == 1){
     highest_priority[1] = 3;
    
    } 
   }    
  }
  
  if (box_array[9].hand_covering == 1) {
   highest_priority[2] = 1;
  
   if (box_array[8].hand_covering == 1){
    highest_priority[2] = 2;
   
    if (box_array[7].hand_covering == 1){
     highest_priority[2] = 3;
    
    } 
   }    
  }

max = highest_priority[k];


for (i = 0; i < 3; i++)
{
    if (highest_priority[i] > max)
    {
        max = (int)highest_priority[i];
        k = i;
    }
}

  sprintf(buffer, "Box w/ hand = %d: %lu\r\n", k);
  SerialOutputString(buffer, &SCI1);


return k;


  
}