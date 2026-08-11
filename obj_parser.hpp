#ifndef _OBJ_PARSER_
#define _OBJ_PARSER_

#include <iostream>
#include <filesystem>

#include "vector_math.hpp"

struct Model_data {
    public:
    Vec_3 *vertices;
    Vec_2 *UV_coordinates;
    Vec_3 *normal;

    Model_data(std::filesystem::path& file_path){

    }

}







#endif
