#include "Rasterizer.h"
#include <algorithm>
#include <cassert>

Rasterizer::Rasterizer(int width, int height): 
m_width(width), m_height(height), m_totalPixels(width*height)
{
    assert(m_width>0 && m_height>0 && "Width and Height dimensions must be positive!");
    m_colorBuffer = new uint32_t[m_totalPixels];
    fillColor(0xFFFFFFFF);
}

Rasterizer::~Rasterizer()
{
    delete[] m_colorBuffer;
}

void Rasterizer::drawPixel(int x, int y, uint32_t color)
{
    m_colorBuffer[x + y*m_width] = color;
}

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{ 
    int sx = (x1 >= x0)? 1: -1;
    int sy = (y1 >= y0)? 1: -1;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int x = x0;
    int y = y0;

    Rasterizer::drawPixel(x0, y0, color);

    if(dx >= dy)
    {
        int d = 2*dy - dx;
        while(x != x1)
        {            
            x += sx;
            if(d <= 0)
            {
                d += 2*dy;
            }
            else
            {
                y += sy;
                d += 2*dy - 2*dx;
            }
            Rasterizer::drawPixel(x, y, color);
        }
    }
    else
    {
        int d = 2*dx - dy;
        while(y != y1)
        {            
            y += sy;
            if(d <= 0)
            {                
                d += 2*dx;
                
            }
            else
            {
                x += sx;
                d += 2*dx - 2*dy;
            }
            Rasterizer::drawPixel(x, y, color);
        }
    }

}

void Rasterizer::drawLine(float x0, float y0, float x1, float y1, uint32_t color)
{
    Rasterizer::drawLine(
        static_cast<int>(std::round(x0)),
        static_cast<int>(std::round(y0)),
        static_cast<int>(std::round(x1)),
        static_cast<int>(std::round(y1)),
        color
    );
}

void Rasterizer::drawTriangle(
    float x0, float y0, 
    float x1, float y1, 
    float x2, float y2, 
    uint32_t color
)
{
    int minX = static_cast<int>(std::floor(std::min({x0, x1, x2})));    
    int maxX = static_cast<int>(std::ceil(std::max({x0, x1, x2})));

    int minY = static_cast<int>(std::floor(std::min({y0, y1, y2})));
    int maxY = static_cast<int>(std::ceil(std::max({y0, y1, y2})));

    for(int y = minY; y <= maxY; y++)
    {
        for(int x = minX; x <= maxX; x++)
        {
            float px = x + 0.5f;
            float py = y + 0.5f;

            float alpha = ((x2-x1)*py - (y2-y1)*px + x1*y2 - x2*y1) / ((x2-x1)*y0 - (y2-y1)*x0 + x1*y2 - x2*y1);
            float beta =  ((x0-x2)*py - (y0-y2)*px + x2*y0 - x0*y2) / ((x0-x2)*y1 - (y0-y2)*x1 + x2*y0 - x0*y2);
            float gamma = ((x1-x0)*py - (y1-y0)*px + x0*y1 - x1*y0) / ((x1-x0)*y2 - (y1-y0)*x2 + x0*y1 - x1*y0);
            
            if(alpha > 0 && beta > 0 && gamma > 0)
            {
                Rasterizer::drawPixel(x, y, color);
            }
        }
    }
}

void Rasterizer::fillColor(uint32_t color)
{
    std::fill_n(m_colorBuffer, m_totalPixels, color);
}