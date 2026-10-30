#ifndef _PARSER_
#define _PARSER_

#include <filesystem>
#include <string>
#include <unordered_map>

struct Model_data; // this is forward declaration to prevent circular dependency , since in our case .hpp dont need to know the size of MOdle_data , it works

struct Parser{
     void parse(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;

     void parse_obj(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;

     void parse_gltf(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;

};
#endif
