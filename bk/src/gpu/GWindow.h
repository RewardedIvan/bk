#pragma once
#include "gpu.h"

namespace bk {
	class GWindow {
	public:
		GWindow(gpu::GPUAPI gapi, const char* name, glm::ivec2 size = glm::ivec2(800, 600));
		~GWindow();

		gpu::GPUAPI getAPI() const;
		glm::ivec2 getWindowSize();

		// whether the window should be open
		bool getOpen() const;
		// should be called every start of a frame
		void RenderStart();
		// should be called every end of a frame
		void RenderEnd();

		gpu::GLFWOpenGLContext* getGLFWContext() const;
	private:
		gpu::GPUContext* gpuctx;
	};
}
