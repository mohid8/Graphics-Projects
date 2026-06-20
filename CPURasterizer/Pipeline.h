#pragma once
#include <GMath.hpp>
#include <Mesh.hpp>
#include "Rasterizer.h"

class Pipeline
{
    public:
    Pipeline(Rasterizer& rasterizer, int width, int height);
    
    void setViewMat(const GMath::Mat4& viewMat);
    void setProjMat(const GMath::Mat4& projMat);

    void renderMesh(const Geometry::Mesh& mesh, const GMath::Mat4& modelMat);

    void updateViewPort(int width, int height);

    private:
    int m_width;
    int m_height;

    Rasterizer& m_rasterizer;
    GMath::Mat4 m_viewMat;
    GMath::Mat4 m_projMat;
    GMath::Mat4 m_projViewMat;
    GMath::Mat4 m_viewPortMat;
    std::vector<GMath::Vec2> m_transformedVertices;
};