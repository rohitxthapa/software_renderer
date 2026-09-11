#include "../include/parser.hpp"
#include "../include/resource_manager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void Parser::parse(const std::filesystem::path &file_path,std::unordered_map<std::string, Model_data> &models) const {
  namespace fs = std::filesystem;
  if (!fs::exists(file_path)) {
    // loose error handling for now
    std::cerr << "the path doesnot exists" << std::endl;
  }
  if (fs::is_regular_file(file_path)) {
    file_check(file_path, models);
  } else if (fs::is_directory(file_path)) {

    for (const auto &entry : fs::directory_iterator(file_path)) {
    file_check(entry, models);
    }
  } else {
    // maybe later we should set some flag here for better error handling
    std::cout << "the path doesnot lead either file or director" << std::endl;
  }
}

void Parser::file_check(const std::filesystem::path &file_path,std::unordered_map<std::string, Model_data> &models) const {
  if (file_path.extension() == ".obj") {
    parse_obj(file_path, models);
  }
  // divide other file type parsing in here
}

void Parser::parse_obj(const std::filesystem::path &file_path,std::unordered_map<std::string, Model_data> &models) const {
  // we will implement function for each file type(if we ever do) and we are
  // taking here refrence to the storage area managed by resource_mangaer
  // parsing for (.obj)
  // currently just vertices,uv,normal and faces  , a lot more remaining (like
  // points , edges , smoothness and a lot more)

  std::ifstream file(file_path);
  if (!file.is_open()) {
    // set some flags
  }

  std::string line;
  Model_data temp;
  std::string name;
  bool flag = false;

  while (std::getline(file, line)) {
    std::stringstream stream(line);
    stream >> line;
    if (line == "o") {
      if (flag) {
        models[name] = std::move(temp);
      }
      stream >> line;
      name = line;
      flag = true;

    } else if (line == "v") {
      stream >> line;
      Float3 point;
      point.x = std::stof(line);
      stream >> line;
      point.y = std::stof(line);
      stream >> line;
      point.z = std::stof(line);

      temp.model_mesh.vertices.push_back(point);
    } else if (line == "vt") {
      stream >> line;
      Float2 point;
      point.x = std::stof(line);
      stream >> line;
      point.y = std::stof(line);

      temp.model_mesh.UV_coord.push_back(point);
    } else if (line == "vn") {
      stream >> line;
      Float3 point;
      point.x = std::stof(line);
      stream >> line;
      point.y = std::stof(line);
      stream >> line;
      point.z = std::stof(line);

      temp.model_mesh.normals.push_back(point);
    } else if (line == "f") {
      Face face;

      while (stream >> line) {
        int ver = -1, uv = -1, nor = -1;
        int flag = 1;
        for (int i = 0; line[i] != '\0'; i++) {
          if (line[i] == '/') {
            flag++;
            continue;
          } else {
            switch (flag) {
            case 1:
              ver = ver * 10 + line[i] - 48;
              break;
            case 2:
              uv = uv * 10 + line[i] - 48;
              break;
            case 3:
              nor = nor * 10 + line[i] - 48;
              break;
            }
          }
        }
        face.vertex_indices.push_back(ver);
        face.UV_indices.push_back(uv);
        face.normal_indices.push_back(nor);
      }
      temp.model_mesh.faces.push_back(face);

      std::vector<Triangle> triangles;
      int first, second, third;
      first = 0;

      for (int i = 0; i < face.vertex_indices.size() - 2; i++) {
        second = i + 1;
        third = i + 2;
        Int3 trivert = {face.vertex_indices[first], face.vertex_indices[second],
                        face.vertex_indices[third]};
        Int3 triuv = {face.UV_indices[first], face.UV_indices[second],
                      face.UV_indices[third]};
        Int3 trinor = {face.normal_indices[first], face.normal_indices[second],
                       face.normal_indices[third]};
        triangles.push_back({trivert, triuv, trinor});
      }
      temp.model_mesh.triangles.insert(temp.model_mesh.triangles.end(),
                                       triangles.begin(), triangles.end());
    }
  }
  if (name.empty()) {
    name = std::to_string(
        models.size()); // temporary solution , so it will work for 255 maybe
  }
  models[name] = std::move(temp);
  file.close();
}
