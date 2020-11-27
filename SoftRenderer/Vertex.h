#pragma once

#include "Mathf.h"

namespace SoftRenderer
{
    class Vertex
    {
    public:
        Vec4 position;
        Vec4 color;
        Vec4 normal;
        Vec4 texcoord;
        Vec4 tangent;
        Vec4 bitangent;
        Vec4 boneIndex;
        Vec4 boneWeight;
    };
}
