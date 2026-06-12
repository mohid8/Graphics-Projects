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

}

void Rasterizer::fillColor(uint32_t color)
{
    std::fill_n(m_colorBuffer, m_totalPixels, color);
}