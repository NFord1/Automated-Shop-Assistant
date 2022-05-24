#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H


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
 floar mid_point_depth;
 int hand_covering; 
 
 
};






#endif