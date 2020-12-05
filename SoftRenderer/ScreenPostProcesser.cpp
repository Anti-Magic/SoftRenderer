#include "ScreenPostProcesser.h"
#include "RasterPipeline.h"
#include "ShaderUnlit.h"

namespace SoftRenderer
{
    static Mesh quad()
    {
        std::vector<Vertex> vertices;
        std::vector<float> rawVertices = {
            // Positions        // Texture Coords
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        std::vector<int> rawIndices = {
            0, 1, 2,
            2, 3, 0
        };
        for (size_t i = 0; i < rawVertices.size(); i += 5)
        {
            Vertex vertex;
            vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
            vertex.texcoord = Vec4(rawVertices[i + 3], rawVertices[i + 4]);
            vertices.push_back(vertex);
        }

        Mesh mesh;
        mesh.vertices = std::move(vertices);
        mesh.indices = std::move(rawIndices);
        return mesh;
    }

    std::unique_ptr<Shader> ScreenPostProcesser::defaultShader(std::make_unique<ShaderUnlit>());
    Mesh ScreenPostProcesser::mesh(quad());

	void ScreenPostProcesser::render(FrameBuffer& src, FrameBuffer& dst, std::unique_ptr<Shader>& shader)
	{
        render(src.color, dst, shader);
	}

    void ScreenPostProcesser::render(std::shared_ptr<Texture2D> src, FrameBuffer& dst, std::unique_ptr<Shader>& shader)
	{
        shader->texture0 = src;
        RasterPipeline::drawTriangles(dst, shader, quad());
	}
}
