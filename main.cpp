#include <iostream>

#include "software_renderer.hpp"

int main() {
    Software_renderer renderer ;
    std::filesystem::path file_path = "../temp_folder/cube.obj";

    renderer.parse(file_path);



    return 0;
}
