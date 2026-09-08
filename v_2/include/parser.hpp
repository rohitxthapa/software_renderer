#ifndef _PARSER_
#define _PARSER_

#include "resource_manager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

struct Parser{
     // Parser (struct because we may implement multiple file type parsing and to combine them in one place)

     void parse(const std::filesystem::path& file_path,Resource_manager& resource_manager) const{
         namespace fs = std::filesystem;
         if(!fs::exists(file_path)){
             // loose error handling for now
             std::cerr<<"the path doesnot exists"<<std::endl;
         }
         if(fs::is_regular_file(file_path)){
             file_check(file_path, resource_manager);
         }else if(fs::is_directory(file_path)){

             for(const auto& entry : fs::directory_iterator(file_path)) {
              file_check(entry, resource_manager);
             }
         }else{
             // maybe later we should set some flag here for better error handling
             std::cout<<"the path doesnot lead either file or director"<<std::endl;
         }

     }
     void file_check(const std::filesystem::path& file_path,Resource_manager& resource_manager) const{
         if(file_path.extension()==".obj"){
             parse_obj(file_path, resource_manager.models);
         }
         // divide other file type parsing in here
     }

     void parse_obj(const std::filesystem::path& file_path,Model_data& models) const {
         // we will implement function for each file type(if we ever do) and we are taking here refrence to the storage area managed by resource_mangaer
         // parsing for (.obj)
         // currently just vertices,uv,normal and faces  , a lot more remaining (like points , edges , smoothness and a lot more)

         std::ifstream file(file_path);
         if(!file.is_open()){
             //set some flags
         }

         auto temp = std::make_unique<Obj>();
         std::string line;

         while(std::getline(file,line)){
             if((line[0] == 'o')&&(temp.vertices.size()>0)){
                 models.
             }

         }

     }
};

#endif
