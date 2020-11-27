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
		Unknown,
		Space,
		Enter,
		//...
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

		std::function<Texture2D(const std::string&, bool isFlipY)> LoadTexture2D;
		std::function<Mesh(const std::string&)> LoadMesh;

		void SetScene(std::shared_ptr<Scene> _scene);
		void loop(float dt);
		void OnKeyDown(KeyCode key);
		void OnKeyUp(KeyCode key);
	};
}
