#pragma once

#include "FrameBuffer.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"
#include "RasterState.h"
#include <memory>

namespace SoftRenderer
{
	class ScreenPostProcesser
	{
	public:
		static void Render(FrameBuffer& src, FrameBuffer& dst, std::unique_ptr<Shader>& shader);
		static void Render(std::shared_ptr<Texture2D> src, FrameBuffer& dst, std::unique_ptr<Shader>& shader);
		static std::unique_ptr<Shader> defaultShader;
	
	private:
		static RasterState rState;
		static Mesh mesh;
	};
}
