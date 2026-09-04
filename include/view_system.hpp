#ifndef _VIEW_SYSTEM_
#define _VIEW_SYSTEM_

#include "custom_types.hpp"

class Camera{
    public:
    Vec_3 position = {1,2,3};
    Vec_3 direction = {0,0,1};
    Vec_3 right = {1,0,0};
    Vec_3 up = {0,1,0};
    float speed_per_sec = 10;
    Mat_4 t_matrix;

    Vec_2 old_mouse_pos = {0,0};
    int change = 1;

};


#endif
