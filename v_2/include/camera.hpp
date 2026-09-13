#ifndef _CAMERA_
#define _CAMERA_

#include "custom_math.hpp"

class Camera {
    private:
        Float3 position ;
        Float3 forward ;
        Float3 right ;
        Float3 up ;
        float speed_per_sec ;
        Float4x4 matrix;
};

#endif
