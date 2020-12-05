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
#include "Rasterization.h"
#include "RasterPipeline.h"
#include "Entity.h"
#include "Camera.h"
#include "ScreenPostProcesser.h"
#include <imgui.h>

namespace SoftRenderer
{
    class SceneTest : public Scene
    {
    public:
        Camera camera;
        Entity box;
        Entity plane;

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
            camera.setPos(Vec3(0, 2, -5));
            camera.lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
            camera.setProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 10.0f));

            lightPos = Vec3(2, 3, -1);
            lightViewMatrix = Mat4::lookAt(lightPos, Vec3(0, 0, 0), Vec3(0, 1, 0));
            lightProjMatrix = Mat4::ortho(10.0f, 10.0f, 0.1f, 40.0f);
            shaderDepth = std::make_unique<ShaderDepth>();

            box.mesh = MeshPrimitive::cube();
            box.setPos(Vec3(0, 0.5f, 0));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            box.shader->texture0 = std::make_shared<Texture2D>(device->LoadTexture2D("res/container.png", true));

            plane.mesh = MeshPrimitive::plane();
            plane.setScale(Vec3(4, 4, 4));
            plane.shader = std::make_unique<ShaderShadowRecv>();
            plane.shader->mvp = plane.getModelMatrix() * camera.getViewProjMatrix();
            plane.shader->uniformM0 = plane.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            plane.shader->texture0 = fboDepth.color;
        }

        void update(float dt) override
        {
            if (device->isKeyPressed(KeyCode::W))
            {
                camera.setPos(camera.getPos() + Vec3(0, 0, dt));
            }
            if (device->isKeyPressed(KeyCode::S))
            {
                camera.setPos(camera.getPos() - Vec3(0, 0, dt));
            }
            if (device->isKeyPressed(KeyCode::A))
            {
                camera.setPos(camera.getPos() - Vec3(dt, 0, 0));
            }
            if (device->isKeyPressed(KeyCode::D))
            {
                camera.setPos(camera.getPos() + Vec3(dt, 0, 0));
            }

            float rate = PI * 0.3f * dt;
            Quat rot = box.getRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate);
            box.setRot(rot);
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            plane.shader->mvp = plane.getModelMatrix() * camera.getViewProjMatrix();

            // shadow map
            fboDepth.clear(Vec4(1, 1, 1, 1));
            shaderDepth->mvp = box.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            RasterPipeline::drawTriangles(fboDepth, shaderDepth, box.mesh);
            //shaderDepth->mvp = plane.getModelMatrix() * lightViewMatrix * lightProjMatrix;
            //Rasterization::drawTriangles(fboDepth, shaderDepth, plane.mesh);

            device->frameBuffer.clear(Vec4(0, 0, 0, 1));
            RasterPipeline::drawTriangles(device->frameBuffer, box.shader, box.mesh);
            RasterPipeline::drawTriangles(device->frameBuffer, plane.shader, plane.mesh);

            //ScreenPostProcesser::render(fboDepth, device->frameBuffer, ScreenPostProcesser::defaultShader);

            ImGui::Begin("Hello, world!");
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", dt * 1000.0f, 1.0f / dt);
            ImGui::End();
        }
    };
}
