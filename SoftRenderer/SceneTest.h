#pragma once

#include "Scene.h"
#include "FrameBuffer.h"
#include "Device.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshPrimitive.h"
#include "ShaderColor.h"
#include "ShaderUnlit.h"
#include "Rasterization.h"

namespace SoftRenderer
{
    class SceneTest : public Scene
    {
    public:
        Mesh mesh;
        std::unique_ptr<Shader> shader;
        Mat4 modelMatrix;
        Mat4 viewMatrix;
        Mat4 projMatrix;

        SceneTest(std::shared_ptr<Device> _device) : Scene(_device)
        {
        }

        void start() override
        {
            //mesh = MeshPrimitive::cube();
            mesh = MeshPrimitive::quad();
            shader = std::make_unique<ShaderUnlit>();
            viewMatrix = Mat4::lookAt(Vec3(0, 0, -2), Vec3(0, 0, 0), Vec3(0, 1, 0));
            projMatrix = Mat4::perspective(PI / 3.0f, device->size.x / device->size.y, 0.1f, 1000.0f);
            modelMatrix = Mat4();

            /*Mat4 rot = Mat4::rotate(Quat::fromAxisAngle(Vec3(0, 1, 0), PI * 0.25f));
            modelMatrix = modelMatrix * rot;*/

            shader->mvp = modelMatrix * viewMatrix * projMatrix;
            shader->texture0 = std::make_shared<Texture2D>(device->LoadTexture2D("res/screen.png", false));
        }

        void update(float dt) override
        {
            float rate = PI * 0.3f * dt;
            Mat4 rot = Mat4::rotate(Quat::fromAxisAngle(Vec3(0, 1, 0), rate));
            modelMatrix = modelMatrix * rot;
            shader->mvp = modelMatrix * viewMatrix * projMatrix;

            device->frameBuffer.clear(Vec4(0, 0, 0, 1));
            Rasterization::drawTriangles(device->frameBuffer, shader, mesh);

            ImGui::Begin("Hello, world!");
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", dt * 1000, 1.0f / dt);
            ImGui::End();
        }
    };
}
