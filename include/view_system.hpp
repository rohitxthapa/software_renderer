#ifndef _VIEW_SYSTEM_
#define _VIEW_SYSTEM_

#include "custom_types.hpp"

class Camera{
    public:
    Vec_3 position;
    Vec_3 direction;
    float speed_per_sec;
    matrix_4x4 t_martix;

};


#endif
