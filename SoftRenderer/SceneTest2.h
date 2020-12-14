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

        void start() override
        {
            camera.setPos(Vec3(0, 0, -1.0f));
            camera.lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
            camera.setProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 100.0f));

            box.mesh = MeshPrimitive::triangle();
            box.setPos(Vec3(0, 0.0f, 0));
            //box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.45));
            //box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.25));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            box.shader->texture0 = device->LoadTexture2D("res/screen.png", true);
        }

        float xxx = 0;

        void update(float dt) override
        {
            updateCameraTransform(dt);

            xxx += 0.3f * dt;
            //box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), xxx));
            /*float rate = PI * 0.3f * dt;
            Quat rot = box.getRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate);
            box.setRot(rot);*/
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();

            device->frameBuffer.clear(Vec4(0, 0, 0, 1));
            RasterPipeline::drawTriangles(device->frameBuffer, box.shader, box.mesh, rState);

            onGUI(dt);
        }

        bool enableRasterModeFill = true;
        bool enableRasterModeWireframe = true;
        bool enableBackFaceCulling = true;
        void onGUI(float dt)
        {
            ImGui::Begin("Debug");
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", dt * 1000, 1.0f / dt);
            if (ImGui::CollapsingHeader("RasterMode", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("Fill", &enableRasterModeFill);
                ImGui::SameLine();
                ImGui::Checkbox("Wireframe", &enableRasterModeWireframe);
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
        void updateCameraTransform(float dt)
        {
            if (device->isKeyPressed(KeyCode::W))
            {
                camera.setPos(camera.getPos() + camera.forward() * dt);
            }
            if (device->isKeyPressed(KeyCode::S))
            {
                camera.setPos(camera.getPos() - camera.forward() * dt);
            }
            if (device->isKeyPressed(KeyCode::A))
            {
                camera.setPos(camera.getPos() - camera.right() * dt);
            }
            if (device->isKeyPressed(KeyCode::D))
            {
                camera.setPos(camera.getPos() + camera.right() * dt);
            }
            if (device->isKeyPressed(KeyCode::Q))
            {
                camera.setPos(camera.getPos() - camera.up() * dt);
            }
            if (device->isKeyPressed(KeyCode::E))
            {
                camera.setPos(camera.getPos() + camera.up() * dt);
            }

            /*if (device->isKeyPressed(KeyCode::W))
            {
                camera.setPos(camera.getPos() + Vec3(0, 0, 1) * dt);
            }
            if (device->isKeyPressed(KeyCode::S))
            {
                camera.setPos(camera.getPos() - Vec3(0, 0, 1) * dt);
            }
            if (device->isKeyPressed(KeyCode::A))
            {
                camera.setPos(camera.getPos() - Vec3(1, 0, 0) * dt);
            }
            if (device->isKeyPressed(KeyCode::D))
            {
                camera.setPos(camera.getPos() + Vec3(1, 0, 0) * dt);
            }
            if (device->isKeyPressed(KeyCode::Q))
            {
                camera.setPos(camera.getPos() - Vec3(0, 1, 0) * dt);
            }
            if (device->isKeyPressed(KeyCode::E))
            {
                camera.setPos(camera.getPos() + Vec3(0, 1, 0) * dt);
            }*/

            if (device->isMousePressed(MouseButton::Left))
            {
                if (mousePosLast.x < 0)
                {
                    mousePosLast = device->getMousePos();
                }
                else
                {
                    Vec2 dlt = device->getMousePos() - mousePosLast;
                    cameraRotH = fmod(cameraRotH + dlt.x * dt * PI * 0.05f, 2.0f * PI);
                    cameraRotV = fmod(cameraRotV + dlt.y * dt * PI * 0.05f, 2.0f * PI);
                    camera.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), cameraRotH) * Quat::fromAxisAngle(Vec3(1, 0, 0), cameraRotV));
                    /*camera.setRot(camera.getRot()
                        * Quat::fromAxisAngle(Vec3(0, 1, 0), dlt.x * dt * PI * 0.05f)
                        * Quat::fromAxisAngle(Vec3(1, 0, 0), dlt.y * dt * PI * 0.05f)
                    );*/
                    mousePosLast = device->getMousePos();
                }
            }
            else
            {
                mousePosLast = Vec2(-1, -1);
            }
        }
    };
}
