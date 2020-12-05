#pragma once

#include "Mesh.h"
#include <vector>

namespace SoftRenderer
{
    class MeshPrimitive
    {
    public:
        static Mesh cube()
        {
            std::vector<Vertex> vertices;
            std::vector<float> rawVertices = {
                // Positions         // Normals   // Texture Coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            };
            for (size_t i = 0; i < rawVertices.size(); i += 8)
            {
                Vertex vertex;
                vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
                vertex.normal = Vec4(rawVertices[i + 3], rawVertices[i + 4], rawVertices[i + 5], 0);
                vertex.texcoord = Vec4(rawVertices[i + 6], rawVertices[i + 7]);
                vertices.push_back(vertex);
            }

            Mesh mesh;
            mesh.vertices = std::move(vertices);
            return mesh;
        }

        static Mesh quad()
        {
            std::vector<Vertex> vertices;
            std::vector<float> rawVertices = {
                // Positions        // Normals         // Texture Coords
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.0f,   0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            };
            std::vector<int> rawIndices = { 
                0, 1, 2, 
                2, 3, 0 
            };
            for (size_t i = 0; i < rawVertices.size(); i += 8)
            {
                Vertex vertex;
                vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
                vertex.normal = Vec4(rawVertices[i + 3], rawVertices[i + 4], rawVertices[i + 5], 0);
                vertex.texcoord = Vec4(rawVertices[i + 6], rawVertices[i + 7]);
                vertices.push_back(vertex);
            }

            Mesh mesh;
            mesh.vertices = std::move(vertices);
            mesh.indices = std::move(rawIndices);
            return mesh;
        }

        static Mesh plane()
        {
            std::vector<Vertex> vertices;
            std::vector<float> rawVertices = {
                // Positions        // Normals         // Texture Coords
                -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.0f, 0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                0.5f, 0.0f, 0.5f,   0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.0f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            };
            std::vector<int> rawIndices = {
                0, 1, 2,
                2, 3, 0
            };
            for (size_t i = 0; i < rawVertices.size(); i += 8)
            {
                Vertex vertex;
                vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
                vertex.normal = Vec4(rawVertices[i + 3], rawVertices[i + 4], rawVertices[i + 5], 0);
                vertex.texcoord = Vec4(rawVertices[i + 6], rawVertices[i + 7]);
                vertices.push_back(vertex);
            }

            Mesh mesh;
            mesh.vertices = std::move(vertices);
            mesh.indices = std::move(rawIndices);
            return mesh;
        }

        static Mesh triangle()
        {
            std::vector<Vertex> vertices;
            std::vector<float> rawVertices = {
                // Positions        // Normals         // Texture Coords
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.0f,   0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            };
            std::vector<int> rawIndices = {
                0, 1, 2,
            };
            for (size_t i = 0; i < rawVertices.size(); i += 8)
            {
                Vertex vertex;
                vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
                vertex.normal = Vec4(rawVertices[i + 3], rawVertices[i + 4], rawVertices[i + 5], 0);
                vertex.texcoord = Vec4(rawVertices[i + 6], rawVertices[i + 7]);
                vertices.push_back(vertex);
            }

            Mesh mesh;
            mesh.vertices = std::move(vertices);
            mesh.indices = std::move(rawIndices);
            return mesh;
        }

        static Mesh skybox()
        {
            std::vector<Vertex> vertices;
            std::vector<float> rawVertices = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
            };
            for (size_t i = 0; i < rawVertices.size(); i += 3)
            {
                Vertex vertex;
                vertex.position = Vec4(rawVertices[i], rawVertices[i + 1], rawVertices[i + 2], 1);
                vertices.push_back(vertex);
            }

            Mesh mesh;
            mesh.vertices = std::move(vertices);
            return mesh;
        }
    };
}
