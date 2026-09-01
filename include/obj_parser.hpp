#ifndef _OBJ_PARSER_
#define _OBJ_PARSER_

#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "custom_types.hpp"

struct Obj_parser {

    Model_data parse(std::filesystem::path& file_path){
        std::ifstream file(file_path); // opening the file
        if(!file.is_open()){
            throw std::runtime_error("model file couldn't open");
        }
        // std::vector<Model_data> models; // the vector that is to be returned

        std::string line; // string to store the content of a line which align with how .obj stores data

        Model_data model; // model to pushback to eh resultant vector
        while(std::getline(file,line)){
            // a single file can have multiple models
            // if(line[0]== 'o' && model.vertices.size()!=0){
            //     models.push_back(model);
            //     model.vertices.clear();
            //     model.UV_coord.clear();
            //     model.normals.clear();
            // }
            // if(line[0]=='#') continue; // skip comments
            std::stringstream stream(line);
            stream>>line;
            if(line == "v"){
                stream>>line;
                Vec_3 point;
                point.x = std::stof(line);
                stream>>line;
                point.y = std::stof(line);
                stream>>line;
                point.z = std::stof(line);
                // there has been some problem with measurement , idk what a unit shout be
                point.x *= 100; point.y *= 100; point.z *= 100;

                model.vertices.push_back(point);
            }else if(line == "vt"){
                stream>>line;
                Vec_2 point;
                point.x = std::stof(line);
                stream>>line;
                point.y = std::stof(line);

                model.UV_coord.push_back(point);
            }else if(line == "vn"){
                stream>>line;
                Vec_3 point;
                point.x = std::stof(line);
                stream>>line;
                point.y = std::stof(line);
                stream>>line;
                point.z = std::stof(line);

                model.normals.push_back(point);
            }else if(line == "f"){
                std::vector<Face_indices> face;
                while(stream>>line){
                    int ver = 0 , uv = 0 , nor = 0 ;
                    int flag = 1 ;
                    for(int i = 0 ; line[i] != '\0' ; i++){
                        if(line[i] == '/'){
                            flag++;
                            continue;
                        }else{
                        switch (flag) {
                            case 1:
                                ver = ver*10 + line[i] - 48;
                                break;
                            case 2:
                                uv = uv * 10 + line[i] - 48;
                                break;
                            case 3:
                                nor = nor*10 + line[i] - 48;
                                break;
                        }
                    }
                    }
                    face.push_back({ver,uv,nor});
                }
                model.faces.push_back(face);
                transform_normal_indices_to_triangle_indices(face,model);
            }
        }
        // models.push_back(model);

        file.close();
        return model;
    }

    void transform_normal_indices_to_triangle_indices(std::vector<Face_indices> face,Model_data& model){
        int i = 0;
        Face_indices i1 = face.at(i);
        i1.a--; i1.b--; i1.c--;
        i++;
        Face_indices i2 = face.at(i);
        i2.a--; i2.b--; i2.c--;
        i++;
        while(i<face.size()){
            Face_indices temp = face.at(i);
            temp.a--; temp.b--; temp.c--;
            model.triangle_indices.push_back({i1,i2,temp});
            i2 = temp;
            i++;
        }
    }
};

#endif
