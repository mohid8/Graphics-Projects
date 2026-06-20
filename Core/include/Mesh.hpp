#pragma once

#include "Vertex.hpp"
#include <vector>


namespace Geometry
{
    struct Mesh
    {
        std::vector<Geometry::Vertex> vertexList;
        std::vector<uint32_t> indexList;
    };

    bool loadMeshFromOBJ(const std::string& filePath, Mesh& outMesh);
}