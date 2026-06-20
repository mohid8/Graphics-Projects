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
    const Geometry::Vertex& v0, 
    const Geometry::Vertex& v1, 
    const Geometry::Vertex& v2
)
{
    float x0 = v0.pos.x; float x1 = v1.pos.x; float x2 = v2.pos.x;
    float y0 = v0.pos.y; float y1 = v1.pos.y; float y2 = v2.pos.y;

    int minX = static_cast<int>(std::floor(std::min({x0, x1, x2})));    
    int maxX = static_cast<int>(std::ceil(std::max({x0, x1, x2})));

    int minY = static_cast<int>(std::floor(std::min({y0, y1, y2})));
    int maxY = static_cast<int>(std::ceil(std::max({y0, y1, y2})));

    float aAlpha = x2-x1; float aBeta = x0-x2; float aGamma = x1-x0;
    float bAlpha = y2-y1; float bBeta = y0-y2; float bGamma = y1-y0;
    float cAlpha = x1*y2 - x2*y1; float cBeta = x2*y0 - x0*y2; float cGamma = x0*y1 - x1*y0;  

    float fAlpha = aAlpha*y0 - bAlpha*x0 + cAlpha;
    float fBeta  = aBeta*y1  - bBeta*x1  + cBeta;
    float fGamma = aGamma*y2 - bGamma*x2 + cGamma;
    
    float aAlphaChange = aAlpha/fAlpha;
    float bAlphaChange = bAlpha/fAlpha;
    float cAlphaChange = cAlpha/fAlpha;
    
    float aBetaChange = aBeta/fBeta;
    float bBetaChange = bBeta/fBeta;
    float cBetaChange = cBeta/fBeta;

    float aGammaChange = aGamma/fGamma;
    float bGammaChange = bGamma/fGamma;
    float cGammaChange = cGamma/fGamma;

    float pX = minX + 0.5f;
    float pY = minY + 0.5f;

    float startAlpha = aAlphaChange*pY - bAlphaChange*pX + cAlphaChange;
    float startBeta  = aBetaChange*pY  - bBetaChange*pX  + cBetaChange;
    float startGamma = aGammaChange*pY - bGammaChange*pX + cGammaChange;

    float fAlphaTopLeft = fAlpha * (aAlpha*(-1.0f) - bAlpha*(-2.0f) + cAlpha);
    float fBetaTopLeft  = fBeta * (aBeta*(-1.0f) - bBeta*(-2.0f) + cBeta);
    float fGammaTopLeft = fGamma * (aGamma*(-1.0f) - bGamma*(-2.0f) + cGamma);

    bool isAlphaTopLeft = fAlphaTopLeft > 0;
    bool isBetaTopLeft  = fBetaTopLeft > 0;
    bool isGammaTopLeft = fGammaTopLeft > 0;
   
    for(int y = minY; y <= maxY; y++)
    {
        float alpha = startAlpha;
        float beta  = startBeta;
        float gamma = startGamma;
        for(int x = minX; x <= maxX; x++)
        {                     
            if(alpha >= 0 && beta >= 0 && gamma >= 0)
            {
                if((alpha > 0 || isAlphaTopLeft) && 
                   (beta  > 0 || isBetaTopLeft ) &&
                   (gamma > 0 || isGammaTopLeft)
                )
                {
                    float r = (alpha*v0.color.r + beta*v1.color.r + gamma*v2.color.r);
                    float g = (alpha*v0.color.g + beta*v1.color.g + gamma*v2.color.g);
                    float b = (alpha*v0.color.b + beta*v1.color.b + gamma*v2.color.b);
                    float a = (alpha*v0.color.a + beta*v1.color.a + gamma*v2.color.a);

                    uint32_t color = (static_cast<uint32_t>(a * 255.0f) << 24) |
                                     (static_cast<uint32_t>(b * 255.0f) << 16) |
                                     (static_cast<uint32_t>(g * 255.0f) << 8 ) |
                                     (static_cast<uint32_t>(r * 255.0f));
                    Rasterizer::drawPixel(x, y, color);
                }                
            }
            alpha -= bAlphaChange;
            beta  -= bBetaChange;
            gamma -= bGammaChange;
        }
        startAlpha += aAlphaChange;
        startBeta  += aBetaChange;
        startGamma += aGammaChange;
    }
}

void Rasterizer::fillColor(uint32_t color)
{
    std::fill_n(m_colorBuffer, m_totalPixels, color);
}