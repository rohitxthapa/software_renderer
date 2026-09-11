#ifndef _PARSER_
#define _PARSER_

#include <filesystem>
#include <string>
#include <unordered_map>

struct Model_data; // this is forward declaration to prevent circular dependency , since in our case .hpp dont need to know the size of MOdle_data , it works

struct Parser{
     // Parser (struct because we may implement multiple file type parsing and to combine them in one place)

     void parse(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;
     // loose error handling is used here , need to improve

     void file_check(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;
     //currently it only calls parse_obj

     void parse_obj(const std::filesystem::path& file_path,std::unordered_map<std::string,Model_data>& models)const;
         // we will implement function for each file type(if we ever do) and we are taking here refrence to the storage area managed by resource_mangaer
         // parsing for (.obj)
         // currently just vertices,uv,normal and faces  , a lot more remaining (like points , edges , smoothness and a lot more)
};
#endif
