#define TINYOBJLOADER_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION

#include "../dependency/tinyobjloader/tiny_obj_loader.h"
#include "../dependency/cgltf/cgltf.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/custom_math.hpp"
#include "../include/parser.hpp"
#include "../include/resource_manager.hpp"

void Parser::parse(const std::filesystem::path &file_path,std::unordered_map<std::string, Model_data> &models) const {
  namespace fs = std::filesystem;
  if (!fs::exists(file_path)) {
    // loose error handling for now
    std::cerr << "The path doesnot exists" << std::endl;
  }
  if (fs::is_regular_file(file_path)) {
    if (file_path.extension() == ".obj") {
        parse_obj(file_path, models);
    }else if(file_path.extension() == ".gltf" || file_path.extension() == ".glb") {
        parse_gltf(file_path,models);
    }else {
        std::cout<< "File type not supported" <<std::endl;
    }
  } else {
    std::cout << "The path is neither a file" << std::endl;
  }
}

void Parser::parse_obj(const std::filesystem::path &file_path, std::unordered_map<std::string, Model_data> &models) const {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  std::string base_dir = file_path.parent_path().string();
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file_path.string().c_str(), base_dir.c_str(), true);

  if(!ret){
      std::cerr << "TinyOBJ error: " << err << std::endl;
      return;
  }

  for(const auto &shape : shapes) {
      Model_data temp;
      Mesh_data &mesh = temp.model_mesh;
      temp.have_animation_or_skeleton = false;
      temp.show = false;

      size_t index_offset = 0;
      // Loop over faces (polygons)
      for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
          size_t fv = size_t(shape.mesh.num_face_vertices[f]); // Usually 3 for triangles

          // Assuming your software renderer only handles triangles here
          if (fv == 3) {
              Triangle t;
              int *vert[3] = {&t.vertex_indices.x,&t.vertex_indices.y,&t.vertex_indices.z};
              int *uv_coord[3] = {&t.UV_indices.x,&t.UV_indices.y,&t.UV_indices.z};
              int *nor[3] = {&t.normal_indices.x,&t.normal_indices.y,&t.normal_indices.z};

              for (size_t v = 0; v < 3; v++) {
                  tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                  // 1. Handle Position
                  Float3 position(
                      attrib.vertices[3 * size_t(idx.vertex_index) + 0],
                      attrib.vertices[3 * size_t(idx.vertex_index) + 1],
                      attrib.vertices[3 * size_t(idx.vertex_index) + 2]
                  );
                  mesh.vertices.push_back(position);
                  *vert[v] = mesh.vertices.size() - 1;

                  // 2. Handle Texcoords (UVs)
                  if (idx.texcoord_index >= 0) {
                      Float2 uv(
                          attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
                          attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
                      );
                      mesh.UV_coord.push_back(uv);
                      *uv_coord[v] = mesh.UV_coord.size() - 1;
                  } else {
                      *uv_coord[v] = -1; // No UV data for this vertex
                  }

                  // 3. Handle Normals
                  if (idx.normal_index >= 0) {
                      Float3 normal(
                          attrib.normals[3 * size_t(idx.normal_index) + 0],
                          attrib.normals[3 * size_t(idx.normal_index) + 1],
                          attrib.normals[3 * size_t(idx.normal_index) + 2]
                      );
                      mesh.normals.push_back(normal);
                      *nor[v] = mesh.normals.size() - 1;
                  } else {
                      *nor[v] = -1; // No Normal data for this vertex
                  }
              }
              mesh.triangles.push_back(t);
          }
          index_offset += fv;
      }

      std::string name = shape.name.empty() ? std::to_string(models.size()) : shape.name;
      models[name] = std::move(temp);
  }
}


void Parser::parse_gltf(const std::filesystem::path &file_path,
                        std::unordered_map<std::string, Model_data> &models) const {
  std::string path = file_path.string();
  cgltf_options options{};
  cgltf_data *data = nullptr;

  if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success) {
    std::cerr << "cgltf: failed to parse " << path << std::endl;
    return;
  }
  if (cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success) {
    std::cerr << "cgltf: failed to load buffers" << std::endl;
    cgltf_free(data);
    return;
  }

  for (cgltf_size m = 0; m < data->meshes_count; ++m) {
    const cgltf_mesh &gmesh = data->meshes[m];
    Model_data model;
    model.show = false;
    model.have_animation_or_skeleton = data->skins_count > 0 || data->animations_count > 0;
    Mesh_data &mesh = model.model_mesh;

    for (cgltf_size p = 0; p < gmesh.primitives_count; ++p) {
      const cgltf_primitive &prim = gmesh.primitives[p];
      if (prim.type != cgltf_primitive_type_triangles) continue;

      const cgltf_accessor *pos = nullptr, *nor = nullptr, *uv = nullptr;
      for (cgltf_size a = 0; a < prim.attributes_count; ++a) {
        const cgltf_attribute &at = prim.attributes[a];
        if (at.index != 0) continue;
        if (at.type == cgltf_attribute_type_position) pos = at.data;
        else if (at.type == cgltf_attribute_type_normal) nor = at.data;
        else if (at.type == cgltf_attribute_type_texcoord) uv = at.data;
      }
      if (!pos) continue;

      int vbase = (int)mesh.vertices.size();
      int ubase = (int)mesh.UV_coord.size();
      int nbase = (int)mesh.normals.size();

      for (cgltf_size i = 0; i < pos->count; ++i) {
        float v[3];
        cgltf_accessor_read_float(pos, i, v, 3);
        mesh.vertices.push_back(Float3(v[0], v[1], v[2]));
      }
      if (nor) for (cgltf_size i = 0; i < nor->count; ++i) {
        float v[3];
        cgltf_accessor_read_float(nor, i, v, 3);
        mesh.normals.push_back(Float3(v[0], v[1], v[2]));
      }
      if (uv) for (cgltf_size i = 0; i < uv->count; ++i) {
        float v[2];
        cgltf_accessor_read_float(uv, i, v, 2);
        Float2 t;
        t.x = v[0]; t.y = v[1];
        mesh.UV_coord.push_back(t);
      }

      cgltf_size index_count = prim.indices ? prim.indices->count : pos->count;
      for (cgltf_size i = 0; i + 2 < index_count; i += 3) {
        int id[3];
        for (int k = 0; k < 3; ++k)
          id[k] = prim.indices ? (int)cgltf_accessor_read_index(prim.indices, i + k)
                               : (int)(i + k);

        Triangle t;
        t.vertex_indices = {vbase + id[0], vbase + id[1], vbase + id[2]};
        t.UV_indices     = uv  ? Int3{ubase + id[0], ubase + id[1], ubase + id[2]} : Int3{-1,-1,-1};
        t.normal_indices = nor ? Int3{nbase + id[0], nbase + id[1], nbase + id[2]} : Int3{-1,-1,-1};
        mesh.triangles.push_back(t);
      }
    }

    std::string name = gmesh.name ? gmesh.name : std::to_string(models.size());
    models[name] = std::move(model);
  }
  cgltf_free(data);
}
