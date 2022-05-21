#ifndef HAND_LOCATION_H
#define HAND_LOCAITON_H


// Generic struct for the boxes 

// For reference:
//   ___________
//  |   |   |   |
//  | 1 | 2 | 3 |
//  |---|---|---|
//  |   |   |   |   <--- Hand goes this way
//  | 4 | 5 | 6 |
//  |---|---|---|
//  | 7 | 8 | 9 |
//  |___|___|___|


struct box{
 
 int box_number;
 // Mid-point will have to be added after Job 2 is done
 float mid_point_x;
 float mid_point_y;
 int hand_covering; 
 
 
};

int user_selection_box_number(box* box_array);
void hand_convered_boxes(box* box_array);
int highest_box_probability(box* box_array);



#endif

