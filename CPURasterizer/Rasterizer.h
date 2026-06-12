#pragma once
#include <GMath.hpp>

class Rasterizer
{
    public:
    Rasterizer(int width, int height);
    ~Rasterizer();

    void fillColor(uint32_t color);
    void drawPixel(int x, int y, uint32_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
    const uint32_t* getColorBuffer() const{return m_colorBuffer;}

    private:
    int m_width;
    int m_height;
    int m_totalPixels;
    uint32_t* m_colorBuffer;
};