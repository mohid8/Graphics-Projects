#pragma once

#include "GMath.hpp"

namespace Geometry
{
    struct Vertex
    {
        GMath::Vec4 pos;
        GMath::Vec4 color;
        GMath::Vec2 texCoord;
    };
}