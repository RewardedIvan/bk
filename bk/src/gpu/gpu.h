#pragma once
#include <GLFW/glfw3.h>

namespace bk::gpu {
	enum GPUAPI {
		GLFWOpenGL
	};

	void InitializeAPI(GPUAPI api);
	void UninitializeAPI(GPUAPI api);

	struct GPUContext {
		GPUContext(GPUAPI api);
		GPUAPI getAPI() const;
	private:
		GPUAPI api;
	};

	struct GLFWOpenGLContext : GPUContext {
		GLFWOpenGLContext(GLFWwindow* window);
		GLFWwindow* window;
	};
}
