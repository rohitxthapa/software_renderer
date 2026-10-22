#ifndef _CAMERA_
#define _CAMERA_

#include "custom_math.hpp"
#include "state_type.hpp"

class Camera {
    public :
        Float3 position ;
        Float3 forward ;
        Float3 right ;
        Float3 up ;
        float speed_per_sec ;
        Float4x4 matrix;


        void init();

        void update(Input_state &input_state ,Render_state &render_state ,float delta);

        void close();
};

#endif
