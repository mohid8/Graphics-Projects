#pragma once
#include <GMath.hpp>
#include <Vertex.hpp>

class Rasterizer
{
    public:
    Rasterizer(int width, int height);
    ~Rasterizer();

    void fillColor(uint32_t color);
    void drawPixel(int x, int y, uint32_t color);

    void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
    void drawLine(float x0, float y0, float x1, float y1, uint32_t color);
    
    void drawTriangle(
        const Geometry::Vertex& v0, 
        const Geometry::Vertex& v1, 
        const Geometry::Vertex& v2
    );
    
    const uint32_t* getColorBuffer() const{return m_colorBuffer;}

    private:
    int m_width;
    int m_height;
    int m_totalPixels;
    uint32_t* m_colorBuffer;
};