#ifndef _ENGINE_
#define _ENGINE_

#include "resource_manager.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "window_interface.hpp"
#include "user_interface.hpp"


class Engine{
    private :
        Resource_manager resource;
        Renderer renderer;
        Camera camera;
        Window_interface window_interface;
        User_interface user_interface;


    public :
        void init(){
            window_interface.init();
            resource.init();
            renderer.init();
            camera.init();
            user_interface.init();
        }

        void run();

        void close(){
            camera.close();
            renderer.close();
            resource.close();
            user_interface.close();
            window_interface.close();
        }


};


#endif
