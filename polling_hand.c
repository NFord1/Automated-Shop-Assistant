#include <stdio.h>
#include "polling_hand.h"
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include <stdlib.h>

#define TOTAL_BOXES 9

int check_poll(void){
  int check=0;
  while(check == 0){
    check = poll_hand(box* box_array);
  }
  reutrn 1;
}


int poll_hand(box* box_array){
// This function polls the 3 right hand boxes to determine if a hand has entered the area
 
  int i, j, sum; 
  float distance[10];
  float avg_distance;
  float hand_trigger = 1200;
  
  for(i = 3, i < TOTAL_BOXES, i=i+3){
    // Based on pre-defined mid-point (x,y) move servo to sample the 
    // distance away of the object in the 3 right hand boxes
    setServoPose(box_array[i].mid_point_x, box_array[i].mid_point_y);
    sum = 0;
    for(j = 0, j<10, j++){
      
      // Takes 10 distance measurments and calculates the average to reduce noise
      distance[j] = GetLatestLaserSample(unsigned long *distance);
      sum+= distance[j];
    }
    
    avg_distance = sum/10;
    // If the average distance is closer than the pre-definied target
    // hand is presentt
    if (avg_distance < hand_trigger){ 
      box_array[i].hand_covering = 1;
      return 1;
    }
    else{
      box_array[i].hand_covering = 0;
    } 
  }
  return 0;
}
