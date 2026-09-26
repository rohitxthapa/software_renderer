#ifndef _ENGINE_
#define _ENGINE_

#include "resource_manager.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "interface.hpp"
#include "helper.hpp"


class Engine{
    private :
        Resource_manager resource_manager;
        Renderer renderer;
        Camera camera;
        Interface interface;
        Helper helper;
        double delta;


    public :
        void init();

        void run();

        void close();

};


#endif
