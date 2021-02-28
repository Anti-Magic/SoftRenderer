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

        virtual void Start()
        {
        }

        virtual void Update(float dt)
        {
        }

		virtual void OnGUI(float dt)
		{
		}

        void OnDestroy()
        {
        }
    };
}
