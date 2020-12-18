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

		void SetScene(std::shared_ptr<Scene> _scene);
		void Loop(float dt);
		
		bool IsKeyPressed(KeyCode key);
		bool IsKeyDown(KeyCode key);
		bool IsKeyUp(KeyCode key);
		bool IsMousePressed(MouseButton b);
		bool IsMouseDown(MouseButton b);
		bool IsMouseUp(MouseButton b);
		Vec2 GetMousePos();

		void OnKeyDown(KeyCode key);
		void OnKeyUp(KeyCode key);
		void OnMouseDown(MouseButton b);
		void OnMouseUp(MouseButton b);
		void OnMouseMotion(const Vec2& pos);

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
