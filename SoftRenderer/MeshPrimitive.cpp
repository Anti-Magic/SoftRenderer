#include "MeshPrimitive.h"

namespace SoftRenderer
{
	Mesh MeshPrimitive::Cube()
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

	Mesh MeshPrimitive::Quad()
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

	Mesh MeshPrimitive::Plane()
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

	Mesh MeshPrimitive::Triangle()
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

	Mesh MeshPrimitive::Skybox()
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
}
