#pragma once

#include "Mesh.h"
#include <vector>

namespace SoftRenderer
{
    class MeshPrimitive
    {
    public:
		static Mesh Cube();
		static Mesh Quad();
		static Mesh Plane();
		static Mesh Triangle();
		static Mesh Skybox();
    };
}
