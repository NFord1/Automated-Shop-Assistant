#include <stdio.h>
#include "hand_location.h"
#include "servo.h"
#include "laser.h"
#include "configurator.h"
#include <stdlib.h>

#define TOTAL_BOXES 9
  


void build_box_array() {
  
  int i;
  box box_array[TOTAL_BOXES];
  
  for(i = 0; i < TOTAL_BOXES; i++) {
    box_array[i].box_number = i;    
    
  }
   




}