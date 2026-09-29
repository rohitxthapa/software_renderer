#ifndef _HELPER_
#define _HELPER_

#include <SDL3/SDL_timer.h>
#include <cstdint>

class Helper{
    private :
        double delta;
        uint64_t start;
        uint64_t end;


    public :
        void init ();
        void calculate_delta();
        void wait();
        double get_delta();
};

#endif
