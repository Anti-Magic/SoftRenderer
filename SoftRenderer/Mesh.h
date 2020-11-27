#pragma once

#include "Vertex.h"
#include <vector>

namespace SoftRenderer
{
    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<int> indices;
    };
}
