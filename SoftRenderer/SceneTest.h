#pragma once

#include "Scene.h"
#include "FrameBuffer.h"
#include "Device.h"
#include "Shader.h"
#include "Mesh.h"
#include "TextureCube.h"
#include "MeshPrimitive.h"
#include "ShaderColor.h"
#include "ShaderUnlit.h"
#include "ShaderDepth.h"
#include "ShaderShadowRecv.h"
#include "ShaderSkybox.h"
#include "RasterPipeline.h"
#include "RasterState.h"
#include "Entity.h"
#include "Camera.h"
#include "ScreenPostProcesser.h"
#include <imgui.h>

namespace SoftRenderer
{
    class SceneTest : public Scene
    {
    public:
        RasterState rState;
        Camera camera;
        Entity box;
        Entity plane;
        Entity skybox;

        std::unique_ptr<Shader> shaderDepth;
        FrameBuffer fboDepth;
        Vec3 lightPos;
        Mat4 lightViewMatrix;
        Mat4 lightProjMatrix;

        SceneTest(std::shared_ptr<Device> _device) 
            : Scene(_device)
            , fboDepth(Vec2(512, 512))
        {
        }

        void Start() override
        {
            camera.SetPos(Vec3(0, 1, -4));
            camera.LookAt(Vec3(0, 1, 0), Vec3(0, 1, 0));
            camera.SetProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 100.0f));

            lightPos = Vec3(2, 3, -1);
            lightViewMatrix = Mat4::lookAt(lightPos, Vec3(0, 0, 0), Vec3(0, 1, 0));
            lightProjMatrix = Mat4::ortho(10.0f, 10.0f, 0.1f, 40.0f);
            shaderDepth = std::make_unique<ShaderDepth>();

            box.mesh = MeshPrimitive::Cube();
            box.SetPos(Vec3(0, 0.51f, 0));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.GetModelMatrix() * camera.GetViewProjMatrix();
            box.shader->texture0 = device->LoadTexture2D("res/container.png", true);

            plane.mesh = MeshPrimitive::Plane();
            plane.SetScale(Vec3(4, 4, 4));
            plane.shader = std::make_unique<ShaderShadowRecv>();
            plane.shader->mvp = plane.GetModelMatrix() * camera.GetViewProjMatrix();
            plane.shader->uniformM0 = plane.GetModelMatrix() * lightViewMatrix * lightProjMatrix;
            plane.shader->texture0 = fboDepth.color;

            skybox.mesh = MeshPrimitive::Skybox();
            skybox.shader = std::make_unique<ShaderSkybox>();
            skybox.shader->texture0 = std::make_shared<TextureCube>(
                device->LoadTexture2D("res/skybox2/left.jpg", false),
                device->LoadTexture2D("res/skybox2/right.jpg", false),
                device->LoadTexture2D("res/skybox2/bottom.jpg", false),
                device->LoadTexture2D("res/skybox2/top.jpg", false),
                device->LoadTexture2D("res/skybox2/back.jpg", false),
                device->LoadTexture2D("res/skybox2/front.jpg", false));

            mousePosLast = Vec2(-1, -1);
            cameraRotH = 0;
            cameraRotV = 0;
        }

        void Update(float dt) override
        {
            UpdateCameraTransform(dt);

            float rate = PI * 0.3f * dt;
            box.SetRot(box.GetRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate));
            box.shader->mvp = box.GetModelMatrix() * camera.GetViewProjMatrix();
            plane.shader->mvp = plane.GetModelMatrix() * camera.GetViewProjMatrix();

            //camera.SetRot(camera.GetRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate));

            skybox.SetPos(camera.GetPos());
            skybox.shader->mvp = skybox.GetModelMatrix() * camera.GetViewProjMatrix();

            // shadow map
            fboDepth.Clear(Vec4(1, 1, 1, 1));
            shaderDepth->mvp = box.GetModelMatrix() * lightViewMatrix * lightProjMatrix;
            RasterPipeline::DrawTriangles(fboDepth, shaderDepth, box.mesh, rState);
            //shaderDepth->mvp = plane.GetModelMatrix() * lightViewMatrix * lightProjMatrix;
            //Rasterization::DrawTriangles(fboDepth, shaderDepth, plane.mesh);

            device->frameBuffer.Clear(Vec4(0, 0, 0, 1));
            rState.enableDepthWrite = false;
            //RasterPipeline::DrawTriangles(device->frameBuffer, skybox.shader, skybox.mesh, rState);
            rState.enableDepthWrite = true;
            RasterPipeline::DrawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
            RasterPipeline::DrawTriangles(device->frameBuffer, plane.shader, plane.mesh, rState);

            //rState.enableDepthWrite = true;
            //RasterPipeline::DrawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
            //RasterPipeline::DrawTriangles(device->frameBuffer, plane.shader, plane.mesh, rState);
            //rState.enableDepthTest = false;
            //rState.enableDepthWrite = false;
            //RasterPipeline::DrawTriangles(device->frameBuffer, skybox.shader, skybox.mesh, rState);

            //ScreenPostProcesser::render(fboDepth, device->frameBuffer, ScreenPostProcesser::defaultShader);

            OnGUI(dt);
        }
    
        bool enableRasterModeFill = true;
        bool enableRasterModeWireframe = false;
        bool enableBackFaceCulling = true;
        void OnGUI(float dt)
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
                ImGui::Checkbox("Clipping", &rState.enableClipping);
                ImGui::Checkbox("DepthTest", &rState.enableDepthTest);
                ImGui::Checkbox("DepthWrite", &rState.enableDepthWrite);
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
