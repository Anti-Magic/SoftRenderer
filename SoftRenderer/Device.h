#pragma once

#include <functional>
#include <string>
#include <memory>
#include "Mathf.h"
#include "FrameBuffer.h"
#include "Texture2D.h"
#include "Mesh.h"

namespace SoftRenderer
{
	class Scene;

	enum class KeyCode
	{
		None = 0,
		Unknown,
		Space,
		W,
		S,
		A,
		D,
		Q,
		E,
		//...
		size,
	};

	enum class MouseButton
	{
		None = 0,
		Left,
		Middle,
		Right,
		size,
	};

	class Device
	{
	public:
		Vec2 size;
		FrameBuffer frameBuffer;
		std::shared_ptr<Scene> scene;

		Device(const Vec2& _size);
		Device(const Device&) = delete;
		Device(const Device&&) = delete;

		std::function<std::shared_ptr<Texture2D>(const std::string&, bool isFlipY)> LoadTexture2D;
		std::function<Mesh(const std::string&)> LoadMesh;

		void setScene(std::shared_ptr<Scene> _scene);
		void loop(float dt);
		
		bool isKeyPressed(KeyCode key);
		bool isKeyDown(KeyCode key);
		bool isKeyUp(KeyCode key);
		bool isMousePressed(MouseButton b);
		bool isMouseDown(MouseButton b);
		bool isMouseUp(MouseButton b);
		Vec2 getMousePos();

		void onKeyDown(KeyCode key);
		void onKeyUp(KeyCode key);
		void onMouseDown(MouseButton b);
		void onMouseUp(MouseButton b);
		void onMouseMotion(const Vec2& pos);

	private:
		uint8_t keyDown[(size_t)KeyCode::size];
		uint8_t keyUp[(size_t)KeyCode::size];
		uint8_t keyPressed[(size_t)KeyCode::size];

		uint8_t mouseDown[(size_t)MouseButton::size];
		uint8_t mouseUp[(size_t)MouseButton::size];
		uint8_t mousePressed[(size_t)MouseButton::size];
		Vec2 mousePos;
	};
}
