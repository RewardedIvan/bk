#pragma once

#include "gpu/Context.h"
#include <GLFW/glfw3.h>

namespace bk::gpu::opengl {
	struct GLFWOpenGLContext : Context {
		GLFWOpenGLContext();

		// API initilization
		virtual void InitializeAPI() override;
		virtual void UninitializeAPI() override;

		// window creation
		virtual void CreateWindow(const char* name, gpu::WindowMode wm = gpu::WindowMode::Windowed, glm::ivec2 size = glm::ivec2(800, 600), uint8_t monitor = 0) override;
		virtual void DestroyWindow() override;

		// window getters
		virtual glm::ivec2 getWindowSize() override;
		virtual bool getWindowOpen() override;

		// rendering
		virtual void FrameStart() override;
		virtual void FrameEnd() override;

		GLFWwindow* window = nullptr;
	};
}
