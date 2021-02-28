#pragma once

#include <imgui.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>
#include "Scene.h"
#include "FrameBuffer.h"
#include "Device.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshPrimitive.h"
#include "ShaderColor.h"
#include "ShaderUnlit.h"
#include "ShaderDepth.h"
#include "ShaderShadowRecv.h"
#include "RasterState.h"
#include "RasterPipeline.h"
#include "Entity.h"
#include "Camera.h"
#include "ScreenPostProcesser.h"

namespace SoftRenderer
{
	class SceneSponza : public Scene
	{
	public:
		RasterState rState;
		Camera camera;
		std::vector<Mesh> meshes;

		std::unique_ptr<Shader> shaderDepth;
		FrameBuffer fboDepth;
		Vec3 lightPos;
		Mat4 lightViewMatrix;
		Mat4 lightProjMatrix;

		SceneSponza(std::shared_ptr<Device> _device)
			: Scene(_device)
			, fboDepth(Vec2(1024, 1024))
		{
		}

		void Start() override
		{
			camera.SetPos(Vec3(0, 0, -1.0f));
			camera.LookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
			camera.SetProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 100.0f));

			tinyobj::ObjReaderConfig reader_config;
			reader_config.mtl_search_path = "res"; // Path to material files
			tinyobj::ObjReader reader;
			if (!reader.ParseFromFile("res/sponza.obj", reader_config)) {
				if (!reader.Error().empty()) {
					std::cerr << "TinyObjReader: " << reader.Error();
				}
				exit(1);
			}
			if (!reader.Warning().empty()) {
				std::cout << "TinyObjReader: " << reader.Warning();
			}

			auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();

			meshes.resize(shapes.size());

			// Loop over shapes
			for (size_t s = 0; s < shapes.size(); s++) {
				// Loop over faces(polygon)
				size_t index_offset = 0;
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
					int fv = shapes[s].mesh.num_face_vertices[f];

					// Loop over vertices in the face.
					for (size_t v = 0; v < fv; v++) {
						// access to vertex
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
						tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
						tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
						tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
						tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
						tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
						tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
						tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
						// Optional: vertex colors
						// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
						// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
						// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
					}
					index_offset += fv;

					// per-face material
					shapes[s].mesh.material_ids[f];
				}
			}

			
		}

		void Update(float dt) override
		{
			UpdateCameraTransform(dt);

			device->frameBuffer.Clear(Vec4(0, 0, 0, 1));
			//RasterPipeline::DrawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
		}

		bool enableRasterModeFill = true;
		bool enableRasterModeWireframe = true;
		bool enableBackFaceCulling = true;
		void OnGUI(float dt) override
		{
			ImGui::Begin("Debug");
			ImGui::Text("Average %.3f ms/frame (%.1f FPS)", dt * 1000, 1.0f / dt);
			if (ImGui::CollapsingHeader("RasterMode", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Fill", &enableRasterModeFill);
				ImGui::SameLine();
				ImGui::Checkbox("Wireframe", &enableRasterModeWireframe);
			}
			if (ImGui::CollapsingHeader("RasterMethod", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::RadioButton("HalfSpace", (int*)&rState.rasterMethod, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Scanline", (int*)&rState.rasterMethod, 1);
			}
			if (ImGui::CollapsingHeader("ClippingAndCulling", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("BackFaceCulling", &enableBackFaceCulling);
			}
			ImGui::End();

			if (enableRasterModeFill)
			{
				rState.rasterMode |= RasterMode::Fill;
			}
			else
			{
				rState.rasterMode &= ~RasterMode::Fill;
			}

			if (enableRasterModeWireframe)
			{
				rState.rasterMode |= RasterMode::Wireframe;
			}
			else
			{
				rState.rasterMode &= ~RasterMode::Wireframe;
			}

			if (enableBackFaceCulling)
			{
				rState.cullType = FaceCulling::Back;
			}
			else
			{
				rState.cullType = FaceCulling::None;
			}
		}

		Vec2 mousePosLast;
		float cameraRotH;
		float cameraRotV;
		void UpdateCameraTransform(float dt)
		{
			if (device->IsKeyPressed(KeyCode::W))
			{
				camera.SetPos(camera.GetPos() + camera.Forward() * dt);
			}
			if (device->IsKeyPressed(KeyCode::S))
			{
				camera.SetPos(camera.GetPos() - camera.Forward() * dt);
			}
			if (device->IsKeyPressed(KeyCode::A))
			{
				camera.SetPos(camera.GetPos() - camera.Right() * dt);
			}
			if (device->IsKeyPressed(KeyCode::D))
			{
				camera.SetPos(camera.GetPos() + camera.Right() * dt);
			}
			if (device->IsKeyPressed(KeyCode::Q))
			{
				camera.SetPos(camera.GetPos() - camera.Up() * dt);
			}
			if (device->IsKeyPressed(KeyCode::E))
			{
				camera.SetPos(camera.GetPos() + camera.Up() * dt);
			}

			if (device->IsMousePressed(MouseButton::Left))
			{
				if (mousePosLast.x < 0)
				{
					mousePosLast = device->GetMousePos();
				}
				else
				{
					Vec2 dlt = device->GetMousePos() - mousePosLast;
					cameraRotH = fmod(cameraRotH + dlt.x * dt * PI * 0.05f, 2.0f * PI);
					cameraRotV = fmod(cameraRotV + dlt.y * dt * PI * 0.05f, 2.0f * PI);
					camera.SetRot(Quat::fromAxisAngle(Vec3(0, 1, 0), cameraRotH) * Quat::fromAxisAngle(Vec3(1, 0, 0), cameraRotV));
					mousePosLast = device->GetMousePos();
				}
			}
			else
			{
				mousePosLast = Vec2(-1, -1);
			}
		}
	};
}
