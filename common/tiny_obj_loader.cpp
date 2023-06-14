#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void fillMatrices(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& indices) {
    std::string inputfile = "Arwing.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over vertices
    for (size_t vec_start = 0; vec_start < attrib.vertices.size(); vec_start += 3) {
        vertices.emplace_back(
            attrib.vertices[vec_start],
            attrib.vertices[vec_start + 1],
            attrib.vertices[vec_start + 2]);
    }

    // Loop over normals
    for (size_t norm_start = 0; norm_start < attrib.normals.size(); norm_start += 3) {
        indices.emplace_back(
            attrib.normals[norm_start],
            attrib.normals[norm_start + 1],
            attrib.normals[norm_start + 2]);
    }
}