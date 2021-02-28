#pragma once

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
#include "Entity.h"
#include "ScreenPostProcesser.h"
#include <imgui.h>

namespace SoftRenderer 
{
    class SceneTest2 : public Scene
    {
    public:
        RasterState rState;
        Entity box;
        Camera camera;

        std::unique_ptr<Shader> shaderDepth;
        FrameBuffer fboDepth;
        Vec3 lightPos;
        Mat4 lightViewMatrix;
        Mat4 lightProjMatrix;

        SceneTest2(std::shared_ptr<Device> _device)
            : Scene(_device)
            , fboDepth(Vec2(1024, 1024))
        {
        }

        void Start() override
        {
            camera.SetPos(Vec3(0, 0, -1.0f));
            camera.LookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
            camera.SetProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 100.0f));

            box.mesh = MeshPrimitive::Triangle();
            box.SetPos(Vec3(0, 0.0f, 0));
            //box.SetRot(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.45));
            //box.SetRot(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.25));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.GetModelMatrix() * camera.GetViewProjMatrix();
            box.shader->texture0 = device->LoadTexture2D("res/screen.png", true);
        }

        float xxx = 0;

        void Update(float dt) override
        {
            UpdateCameraTransform(dt);

            xxx += 0.3f * dt;
            //box.SetRot(Quat::fromAxisAngle(Vec3(0, 1, 0), xxx));
            /*float rate = PI * 0.3f * dt;
            Quat rot = box.GetRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate);
            box.SetRot(rot);*/
            box.shader->mvp = box.GetModelMatrix() * camera.GetViewProjMatrix();

            device->frameBuffer.Clear(Vec4(0, 0, 0, 1));
            RasterPipeline::DrawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
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

            /*if (device->IsKeyPressed(KeyCode::W))
            {
                camera.SetPos(camera.GetPos() + Vec3(0, 0, 1) * dt);
            }
            if (device->IsKeyPressed(KeyCode::S))
            {
                camera.SetPos(camera.GetPos() - Vec3(0, 0, 1) * dt);
            }
            if (device->IsKeyPressed(KeyCode::A))
            {
                camera.SetPos(camera.GetPos() - Vec3(1, 0, 0) * dt);
            }
            if (device->IsKeyPressed(KeyCode::D))
            {
                camera.SetPos(camera.GetPos() + Vec3(1, 0, 0) * dt);
            }
            if (device->IsKeyPressed(KeyCode::Q))
            {
                camera.SetPos(camera.GetPos() - Vec3(0, 1, 0) * dt);
            }
            if (device->IsKeyPressed(KeyCode::E))
            {
                camera.SetPos(camera.GetPos() + Vec3(0, 1, 0) * dt);
            }*/

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
                    /*camera.SetRot(camera.GetRot()
                        * Quat::fromAxisAngle(Vec3(0, 1, 0), dlt.x * dt * PI * 0.05f)
                        * Quat::fromAxisAngle(Vec3(1, 0, 0), dlt.y * dt * PI * 0.05f)
                    );*/
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
