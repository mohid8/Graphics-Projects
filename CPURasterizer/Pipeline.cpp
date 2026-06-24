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

void Pipeline::setRenderMode(RenderMode renderMode)
{
    m_renderMode = renderMode;
}

void Pipeline::renderMesh(const Geometry::Mesh& mesh, const GMath::Mat4& modelMat)
{
    m_transformedVertices.clear();
    m_transformedVertices.reserve(mesh.vertexList.size());

    GMath::Mat4 projViewModelMat = m_projViewMat * modelMat;
    
    for(Geometry::Vertex v : mesh.vertexList)
    {
        GMath::Vec4 clipSpace = projViewModelMat*v.pos;
        GMath::Vec4 ndc = clipSpace/clipSpace.w;
        GMath::Vec4 screenSpace = m_viewPortMat*ndc;
        m_transformedVertices.push_back(Geometry::Vertex{screenSpace, v.color, v.texCoord});
    }

    for(int i = 0; i < mesh.indexList.size(); i+=3)
    {
        Geometry::Vertex v0 = m_transformedVertices[mesh.indexList[i]];
        Geometry::Vertex v1 = m_transformedVertices[mesh.indexList[i+1]];
        Geometry::Vertex v2 = m_transformedVertices[mesh.indexList[i+2]];

        if(m_renderMode == RenderMode::WIREFRAME)
        {
            uint32_t color = 0xFF00FF00;
            m_rasterizer.drawLine(
                v0.pos.x, v0.pos.y,
                v1.pos.x, v1.pos.y,
                color
            );
            m_rasterizer.drawLine(
                v1.pos.x, v1.pos.y,
                v2.pos.x, v2.pos.y,
                color
            );
            m_rasterizer.drawLine(
                v2.pos.x, v2.pos.y,
                v0.pos.x, v0.pos.y,
                color
            );
        }
        else if(m_renderMode == RenderMode::SOLID)
        {
            m_rasterizer.drawTriangle(v0, v1, v2);
        }
        
    }
}

