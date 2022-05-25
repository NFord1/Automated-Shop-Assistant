#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#define TOTAL_BOXES 9


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
 float mid_point_depth;
 int hand_covering; 
 
 
};

void build_box_array(struct box box_array[TOTAL_BOXES]);
void set_midpoints_box_array(float home_x, float home_y, struct box* box_array);
float local_average_depth();
float azimuth_calc(float anchor_distance, int scale);


#endif