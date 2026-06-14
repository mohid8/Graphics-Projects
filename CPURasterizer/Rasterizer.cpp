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

void Rasterizer::fillColor(uint32_t color)
{
    std::fill_n(m_colorBuffer, m_totalPixels, color);
}