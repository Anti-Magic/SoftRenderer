#pragma once

#include "Device.h"

namespace SoftRenderer
{
    class Scene
    {
    public:
        std::shared_ptr<Device> device;

        Scene(std::shared_ptr<Device> _device)
            : device(_device)
        {
        }

        virtual void start()
        {
        }

        virtual void update(float dt)
        {
        }

        void stop()
        {
        }
    };
}
