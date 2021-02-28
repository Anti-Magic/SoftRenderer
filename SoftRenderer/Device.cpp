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
		memset(mouseDown, 0, (size_t)MouseButton::size);
		memset(mouseUp, 0, (size_t)MouseButton::size);
		memset(mousePressed, 0, (size_t)MouseButton::size);
	}

	void Device::SetScene(std::shared_ptr<Scene> _scene)
	{
		scene = _scene;
		scene->Start();
	}

	void Device::Loop(float dt)
	{
		if (scene != nullptr)
		{
			scene->Update(dt);
			scene->OnGUI(dt);
		}

		memset(keyDown, 0, (size_t)KeyCode::size);
		memset(keyUp, 0, (size_t)KeyCode::size);
		memset(mouseDown, 0, (size_t)MouseButton::size);
		memset(mouseUp, 0, (size_t)MouseButton::size);
	}

	bool Device::IsKeyPressed(KeyCode key)
	{
		return keyPressed[(size_t)key] == 1;
	}

	bool Device::IsKeyDown(KeyCode key)
	{
		return keyDown[(size_t)key] == 1;
	}

	bool Device::IsKeyUp(KeyCode key)
	{
		return keyUp[(size_t)key] == 1;
	}

	bool Device::IsMousePressed(MouseButton b)
	{
		return mousePressed[(size_t)b] == 1;
	}

	bool Device::IsMouseDown(MouseButton b)
	{
		return mouseDown[(size_t)b] == 1;
	}

	bool Device::IsMouseUp(MouseButton b)
	{
		return mouseUp[(size_t)b] == 1;
	}

	Vec2 Device::GetMousePos()
	{
		return mousePos;
	}

	void Device::OnKeyDown(KeyCode key)
	{
		keyDown[(size_t)key] = 1;
		keyPressed[(size_t)key] = 1;
	}

	void Device::OnKeyUp(KeyCode key)
	{
		keyUp[(size_t)key] = 1;
		keyPressed[(size_t)key] = 0;
	}

	void Device::OnMouseDown(MouseButton b)
	{
		mouseDown[(size_t)b] = 1;
		mousePressed[(size_t)b] = 1;
	}

	void Device::OnMouseUp(MouseButton b)
	{
		mouseUp[(size_t)b] = 1;
		mousePressed[(size_t)b] = 0;
	}

	void Device::OnMouseMotion(const Vec2& pos)
	{
		mousePos = pos;
	}
}
