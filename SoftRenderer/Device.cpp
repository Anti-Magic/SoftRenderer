#include "Device.h"
#include "Scene.h"

namespace SoftRenderer
{
	Device::Device(const Vec2& _size)
		: size(_size)
		, frameBuffer(_size)
	{
	}

	void Device::SetScene(std::shared_ptr<Scene> _scene)
	{
		scene = _scene;
		scene->start();
	}

	void Device::loop(float dt)
	{
		if (scene != nullptr)
		{
			scene->update(dt);
		}
	}

	void Device::OnKeyDown(KeyCode key)
	{

	}

	void Device::OnKeyUp(KeyCode key)
	{

	}
}
