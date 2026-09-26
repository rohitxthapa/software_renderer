#ifndef _CAMERA_
#define _CAMERA_

#include "custom_math.hpp"

class Camera {
    public :
        Float3 position ;
        Float3 forward ;
        Float3 right ;
        Float3 up ;
        float speed_per_sec ;
        Float4x4 matrix;
        float fovy;
        float near_plane;
        float far_plane;
        bool lock;

        void init();

        Float3 get_fovy_near_far_plane();

        void close();
};

#endif
