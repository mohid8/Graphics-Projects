#define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.hpp"
#include "tiny_obj_loader.h"
#include <unordered_map>

namespace Geometry
{
    bool loadMeshFromOBJ(const std::string& filePath, Mesh& outMesh)
    {
        tinyobj::ObjReaderConfig reader_config;
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filePath, reader_config)) 
        {
            if (!reader.Error().empty()) 
            {
                std::cerr << "TinyObjReader error: " << reader.Error() << "\n";
            }
            return false;
        }

        if (!reader.Warning().empty()) 
        {
            std::cout << "TinyObjReader warning: " << reader.Warning() << "\n";
        }

        const auto& attrib = reader.GetAttrib();
        const auto& shapes = reader.GetShapes();

        outMesh.vertexList.clear();
        outMesh.indexList.clear();

        std::unordered_map<int, uint32_t> uniqueVertices;

        for (size_t s = 0; s < shapes.size(); s++) 
        {
            for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) 
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[i];

                auto iterator = uniqueVertices.find(idx.vertex_index);

                if(iterator != uniqueVertices.end())
                {
                    outMesh.indexList.push_back(iterator->second);
                }
                else
                {
                    Geometry::Vertex v;
                    float x = attrib.vertices[3 * size_t(idx.vertex_index)];
                    float y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    float z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    v.pos = { x, y, z, 1.0f };
                    v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                    outMesh.indexList.push_back(static_cast<uint32_t>(outMesh.vertexList.size()));
                    outMesh.vertexList.push_back(v);
                    
                }               
                
            }
        }
        return true;
    }
}