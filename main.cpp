
#include "engine.hpp"

int main() {
    Engine engine ;
    std::filesystem::path file_path = "../temp_folder/cube.obj";

    engine.parse(file_path);
    engine.init();
    engine.run();
    engine.close();

    return 0;
}
