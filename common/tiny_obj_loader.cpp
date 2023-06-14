#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include "VBO.h"
#include <glm/glm.hpp>

std::pair<std::vector<Vertex>, std::vector<GLuint>> loadOBJ(const std::string& filename) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, readerConfig)) {
        if (!reader.Error().empty()) {
            std::cerr << "Failed to load OBJ file: " << reader.Error() << std::endl;
        }
        return { vertices, indices };
    }

    if (!reader.Warning().empty()) {
        std::cout << "Warning loading OBJ file: " << reader.Warning() << std::endl;
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();

    for (const auto& shape : shapes) {
        const auto& mesh = shape.mesh;

        for (size_t i = 0; i < mesh.indices.size(); i++) {
            const tinyobj::index_t& index = mesh.indices[i];

            Vertex vertex;
            vertex.position = glm::vec3(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            );
            vertex.normal = glm::vec3(
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            );
            vertex.color = glm::vec3(
                attrib.colors[3 * index.vertex_index + 0],
                attrib.colors[3 * index.vertex_index + 1],
                attrib.colors[3 * index.vertex_index + 2]
            );
            vertex.texUV = glm::vec2(
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            );

            vertices.push_back(vertex);
            indices.push_back(static_cast<GLuint>(indices.size()));
        }
    }

    return { vertices, indices };
}
