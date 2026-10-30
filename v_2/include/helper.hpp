#ifndef _HELPER_
#define _HELPER_

#include <SDL3/SDL_timer.h>
#include <cstdint>

class Helper{
    private :
        double delta;
        uint64_t start;
        uint64_t end;

    public:
        float frame_time;
        float wait_frame_time;
        float frame_time_100;
        float frame_time_60;
        float frame_time_50;
        float frame_time_30;
        float frame_time_10;

        float fps;

        void init ();
        void calculate_delta();
        void wait();
        double get_delta();
};

#endif
