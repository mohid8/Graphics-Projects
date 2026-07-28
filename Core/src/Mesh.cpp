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

        std::unordered_map<uint64_t, uint32_t> uniqueVertices;

        for (size_t s = 0; s < shapes.size(); s++) 
        {
            for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) 
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[i];
                
                uint64_t vertexKey = (static_cast<uint64_t>(idx.vertex_index) << 32) |
                                     (static_cast<uint64_t>(idx.texcoord_index) & 0xFFFFFFFF);

                auto iterator = uniqueVertices.find(vertexKey);

                if(iterator != uniqueVertices.end())
                {
                    outMesh.indexList.push_back(iterator->second);
                }
                else
                {
                    Geometry::Vertex v;
                    float texU = 0.0f;
                    float texV = 0.0f;                    
                    float x = attrib.vertices[3 * size_t(idx.vertex_index)];
                    float y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    float z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    v.pos = { x, y, z, 1.0f };
                    v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                    if(idx.texcoord_index >= 0)
                    {
                        texU = attrib.texcoords[2 * size_t(idx.texcoord_index)];
                        texV = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    v.texCoord.u = texU;
                    v.texCoord.v = texV;

                    uint32_t newUniqueIndex = static_cast<uint32_t>(outMesh.vertexList.size());

                    outMesh.indexList.push_back(newUniqueIndex);
                    outMesh.vertexList.push_back(v);

                    uniqueVertices[vertexKey] = newUniqueIndex;                 
                }               
                
            }
        }
        return true;
    }
}