#ifndef _PARSER_
#define _PARSER_

#include "resource_manager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Parser{
     // Parser (struct because we may implement multiple file type parsing and to combine them in one place)

     void parse(const std::filesystem::path& file_path,std::unique_ptr<std::vector<Model_data>>& models) const{
         namespace fs = std::filesystem;
         if(!fs::exists(file_path)){
             // loose error handling for now
             std::cerr<<"the path doesnot exists"<<std::endl;
         }
         if(fs::is_regular_file(file_path)){
             file_check(file_path, models);
         }else if(fs::is_directory(file_path)){

             for(const auto& entry : fs::directory_iterator(file_path)) {
              file_check(entry, models);
             }
         }else{
             // maybe later we should set some flag here for better error handling
             std::cout<<"the path doesnot lead either file or director"<<std::endl;
         }

     }
     void file_check(const std::filesystem::path& file_path,std::unique_ptr<std::vector<Model_data>>& models) const{
         if(file_path.extension()==".obj"){
             parse_obj(file_path, models);
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
         // if i use unique pointer i shoudl i shooudl include std::unique_ptr<date_type>& in the parameter
         // and we use make_unique to tell that the pointer is going to be used by only one place
         // and we will need to .push_back(std::move(newptr)) newptr is mady as auto newptr = std::make_unique<data>(data{input,input})
         // we use std::Move to change the ownership of pointer and so the pointer doesnt disolve when we get out of this function
         std::string line;

         while(std::getline(file,line)){
             if((line[0] == 'o')&&(temp->vertices.size()>0)){
                 models.obj_models.push_back(std::move(temp));
             }

         }

     }
};

#endif
