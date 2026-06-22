#include "Pipeline.h"

Pipeline::Pipeline(Rasterizer& rasterizer, int width, int height):
m_rasterizer(rasterizer)
{
    updateViewPort(width, height);
}


void Pipeline::setViewMat(const GMath::Mat4& viewMat)
{
    m_viewMat = viewMat;
    m_projViewMat = m_projMat * m_viewMat;
}

void Pipeline::setProjMat(const GMath::Mat4& projMat)
{
    m_projMat = projMat;
    m_projViewMat = m_projMat * m_viewMat;
}

void Pipeline::updateViewPort(int width, int height)
{
    m_width = width;
    m_height = height;

    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    m_viewPortMat = GMath::Mat4(
        w/2.0f, 0.0f  , 0.0f, w/2.0f,
        0.0f  , -h/2.0f, 0.0f, h/2.0f,
        0.0f  , 0.0f  , 1.0f, 0.0f  ,
        0.0f  , 0.0f  , 0.0f, 1.0f
    );
}

void Pipeline::renderMesh(const Geometry::Mesh& mesh, const GMath::Mat4& modelMat)
{
    m_transformedVertices.clear();
    m_transformedVertices.reserve(mesh.vertexList.size());

    GMath::Mat4 projViewModelMat = m_projViewMat * modelMat;
    uint32_t color = 0xFF00FF00;
    for(Geometry::Vertex v : mesh.vertexList)
    {
        GMath::Vec4 clipSpace = projViewModelMat*v.pos;
        GMath::Vec4 ndc = clipSpace/clipSpace.w;
        GMath::Vec4 screenSpace = m_viewPortMat*ndc;
        m_transformedVertices.push_back(screenSpace.asVec2());
    }

    for(int i = 0; i < mesh.indexList.size(); i+=3)
    {
        GMath::Vec2 v0 = m_transformedVertices[mesh.indexList[i]];
        GMath::Vec2 v1 = m_transformedVertices[mesh.indexList[i+1]];
        GMath::Vec2 v2 = m_transformedVertices[mesh.indexList[i+2]];

        m_rasterizer.drawLine(
            v0.x, v0.y,
            v1.x, v1.y,
            color
        );
        m_rasterizer.drawLine(
            v1.x, v1.y,
            v2.x, v2.y,
            color
        );
        m_rasterizer.drawLine(
            v2.x, v2.y,
            v0.x, v0.y,
            color
        );
    }
}

