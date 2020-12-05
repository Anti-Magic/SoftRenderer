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
#include "Entity.h"
#include "ScreenPostProcesser.h"
#include <imgui.h>

namespace SoftRenderer 
{
    class SceneTest2 : public Scene
    {
    public:
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
            camera.setPos(Vec3(0, 0, -2.0f));
            camera.lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
            camera.setProjMatrix(Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 10.0f));

            box.mesh = MeshPrimitive::triangle();
            box.setPos(Vec3(0, 0.0f, 0));
            //box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.36));
            box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), 7.35));
            box.shader = std::make_unique<ShaderUnlit>();
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();
            box.shader->texture0 = std::make_shared<Texture2D>(device->LoadTexture2D("res/test.jpg", true));
        }

        float xxx = 0;

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

            xxx += 0.3f * dt;
            //box.setRot(Quat::fromAxisAngle(Vec3(0, 1, 0), xxx));
            /*float rate = PI * 0.3f * dt;
            Quat rot = box.getRot() * Quat::fromAxisAngle(Vec3(0, 1, 0), rate);
            box.setRot(rot);*/
            box.shader->mvp = box.getModelMatrix() * camera.getViewProjMatrix();

            device->frameBuffer.clear(Vec4(0, 0, 0, 1));
            RasterPipeline::drawTriangles(device->frameBuffer, box.shader, box.mesh);

            ImGui::Begin("Hello, world!");
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", dt * 1000, 1.0f / dt);
            ImGui::End();
        }
    };
}
