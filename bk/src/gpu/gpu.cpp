#include "pch/pch.h"

#include "gpu.h"
#include "utils/log.h"
#include "input/Input.h"

namespace bk::gpu {
	void InitializeAPI(GPUAPI api) {
		switch (api) {
		case gpu::GLFWOpenGL: {
			glfwSetErrorCallback([](int err, const char* desc) {
				//Log::fatal("[GLFW] %d: %s", 2, err, desc);
				Log::error("[GLFW] %d: %s", err, desc);
			});

			if (!glfwInit())
				Log::fatal("couldn't initalize glfw", 1);

			input::Keys::SetupGLFW();
			break;
		}
		default:
			break;
		}

		// imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// style
		ImGui::StyleColorsDark();
	}

	void UninitializeAPI(GPUAPI api) {
		switch (api) {
		case gpu::GLFWOpenGL:
			glfwTerminate();
			break;
		default:
			break;
		}
	}

	GPUContext::GPUContext(GPUAPI api) : api(api) {
	}

	GPUAPI GPUContext::getAPI() const {
		return api;
	}

	GLFWOpenGLContext::GLFWOpenGLContext(GLFWwindow* window) : GPUContext(GPUAPI::GLFWOpenGL),
		window(window) {
	};
}