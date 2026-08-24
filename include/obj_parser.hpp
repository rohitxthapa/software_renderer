#ifndef _OBJ_PARSER_
#define _OBJ_PARSER_

#include <fstream>
#include <filesystem>

#include "custom_types.hpp"

struct Obj_parser {

    Model_data parse(std::filesystem::path& file_path){
        std::ifstream file(file_path); // opening the file
        if(!file.is_open()){
            throw " model file couldn't open";
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
            if(line[0]=='v'){ //vertices
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
                }
            }else if(line[0]=='f'){
                std::stringstream stream(line);
                stream>>line;

                std::vector<Face_indices> face;

                while(stream>>line){
                    // for now we assume the face have all three part
                    int a,b,c;
                    a = line[0]-48;
                    b = line[2]-48;
                    c = line[4]-48;
                    face.push_back({a,b,c});
                }
                model.faces.push_back(face);
                transform_normal_indices_to_triangle_indices(face,model);
            }
        }
        // models.push_back(model);

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
