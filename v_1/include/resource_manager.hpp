#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "custom_types.hpp"

class Resource_manager{
    public:
        Model_data model;


        Model_data& get_model_refrence (){
            return model;
        }

};

#endif
