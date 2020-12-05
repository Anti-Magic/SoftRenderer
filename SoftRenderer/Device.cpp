#include "Device.h"
#include "Scene.h"

namespace SoftRenderer
{
	Device::Device(const Vec2& _size)
		: size(_size)
		, frameBuffer(_size)
	{
		memset(keyDown, 0, (size_t)KeyCode::size);
		memset(keyUp, 0, (size_t)KeyCode::size);
		memset(keyPressed, 0, (size_t)KeyCode::size);
	}

	void Device::setScene(std::shared_ptr<Scene> _scene)
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

		memset(keyDown, 0, (size_t)KeyCode::size);
		memset(keyUp, 0, (size_t)KeyCode::size);
	}

	bool Device::isKeyPressed(KeyCode key)
	{
		return keyPressed[(size_t)key] == 1;
	}

	bool Device::isKeyDown(KeyCode key)
	{
		return keyDown[(size_t)key] == 1;
	}

	bool Device::isKeyUp(KeyCode key)
	{
		return keyUp[(size_t)key] == 1;
	}

	void Device::onKeyDown(KeyCode key)
	{
		keyDown[(size_t)key] = 1;
		keyPressed[(size_t)key] = 1;
	}

	void Device::onKeyUp(KeyCode key)
	{
		keyUp[(size_t)key] = 1;
		keyPressed[(size_t)key] = 0;
	}
}
