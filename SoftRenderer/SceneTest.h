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

        void start() override
        {
            camera.setPos(Vec3(0, 1, -4));
            camera.lookAt(Vec3(0, 1, 0), Vec3(0, 1, 0));
            camera.setProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 100.0f));

            lightPos = Vec3(2, 3, -1);
            lightViewMatrix = Mat4::lookAt(lightPos, Vec3(0, 0, 0), Vec3(0, 1, 0));
            lightProjMatrix = Mat4::ortho(10.0f, 10.0f, 0.1f, 40.0f);
            shaderDepth = std::make_unique<ShaderDepth>();

            box.mesh = MeshPrimitive::cube();
            box.setPos(Vec3(0, 0.51f, 0));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            box.shader->texture0 = device->LoadTexture2D("res/container.png", true);

            plane.mesh = MeshPrimitive::plane();
            plane.setScale(Vec3(4, 4, 4));
            plane.shader = std::make_unique<ShaderShadowRecv>();
            plane.shader->mvp = plane.getModelMatrix() * camera.getViewProjMatrix();
            plane.shader->uniformM0 = plane.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            plane.shader->texture0 = fboDepth.color;

            skybox.mesh = MeshPrimitive::skybox();
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

        void update(float dt) override
        {
            updateCameraTransform(dt);

            float rate = PI * 0.3f * dt;
            box.setRot(box.getRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate));
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            plane.shader->mvp = plane.getModelMatrix() * camera.getViewProjMatrix();

            //camera.setRot(camera.getRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate));

            skybox.setPos(camera.getPos());
            skybox.shader->mvp = skybox.getModelMatrix() * camera.getViewProjMatrix();

            // shadow map
            fboDepth.clear(Vec4(1, 1, 1, 1));
            shaderDepth->mvp = box.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            RasterPipeline::drawTriangles(fboDepth, shaderDepth, box.mesh, rState);
            //shaderDepth->mvp = plane.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            //Rasterization::drawTriangles(fboDepth, shaderDepth, plane.mesh);

            device->frameBuffer.clear(Vec4(0, 0, 0, 1));
            rState.enableDepthWrite = false;
            //RasterPipeline::drawTriangles(device->frameBuffer, skybox.shader, skybox.mesh, rState);
            rState.enableDepthWrite = true;
            RasterPipeline::drawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
            RasterPipeline::drawTriangles(device->frameBuffer, plane.shader, plane.mesh, rState);

            //rState.enableDepthWrite = true;
            //RasterPipeline::drawTriangles(device->frameBuffer, box.shader, box.mesh, rState);
            //RasterPipeline::drawTriangles(device->frameBuffer, plane.shader, plane.mesh, rState);
            //rState.enableDepthTest = false;
            //rState.enableDepthWrite = false;
            //RasterPipeline::drawTriangles(device->frameBuffer, skybox.shader, skybox.mesh, rState);

            //ScreenPostProcesser::render(fboDepth, device->frameBuffer, ScreenPostProcesser::defaultShader);

            onGUI(dt);
        }
    
        bool enableRasterModeFill = true;
        bool enableRasterModeWireframe = false;
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
