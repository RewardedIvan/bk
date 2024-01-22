#pragma once

#include "WindowMode.h"

namespace bk::gpu {
	enum API {
		GLFWOpenGL
	};

	struct Context { // abstract
		Context(API api);
		API getAPI() const;

		// API initilization
		// note: call these base functions after your initializing your api
		virtual void InitializeAPI();
		virtual void UninitializeAPI() = 0;

		// window creation
#undef CreateWindow // windows.h
		virtual void CreateWindow(const char* name, gpu::WindowMode wm = gpu::WindowMode::Windowed, glm::ivec2 size = glm::ivec2(800, 600), uint8_t monitor = 0) = 0;
		virtual void DestroyWindow() = 0;

		// window getters
		virtual glm::ivec2 getWindowSize() = 0;
		virtual bool getWindowOpen() = 0;

		// rendering
		virtual void FrameStart() = 0;
		virtual void FrameEnd() = 0;
	private:
		API api;
	};
}
