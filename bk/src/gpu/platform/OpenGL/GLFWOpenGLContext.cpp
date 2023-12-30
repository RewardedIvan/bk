#include "pch/pch.h"

#include "GLFWOpenGLContext.h"
#include "utils/log.h"
#include "events/GLFWInputEvent.h"

namespace bk::gpu::opengl {
	GLFWOpenGLContext::GLFWOpenGLContext() : gpu::Context(API::GLFWOpenGL) {
	}

	// API initilization
	void GLFWOpenGLContext::InitializeAPI() {
		glfwSetErrorCallback([](int err, const char* desc) {
			Log::fatal("[GLFW] %d: %s", 2, err, desc);
		});

		if (!glfwInit())
			Log::fatal("couldn't initalize glfw", 1);

		gpu::Context::InitializeAPI();
	}

	void GLFWOpenGLContext::UninitializeAPI() {
		glfwTerminate();

		gpu::Context::UninitializeAPI();
	}

	// window creation
	void GLFWOpenGLContext::CreateWindow(const char* name, gpu::WindowMode wm, glm::ivec2 size, uint8_t monitor) {
		// opengl version & profile
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window
		window = glfwCreateWindow(size.x, size.y, name, nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // vsync

		// glew
		GLenum err = glewInit();
		if (err != GLEW_OK)
			Log::fatal("couldn't initialize glew %d (0x0%x)", err, err);

		Log::info("using GLFW + OpenGL %s", glGetString(GL_VERSION));

#ifdef BK_INCLUDE_IMGUI
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
#endif

		// input event
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			events::GLFWInput* eventData = new events::GLFWInput();
			eventData->window = window;
			eventData->key = key;
			eventData->scancode = scancode;
			eventData->action = action;
			eventData->mods = mods;

			events::EventManager::broadcast(events::GLFWInput::id, eventData);
		});
	}

	void GLFWOpenGLContext::DestroyWindow() {
		glfwDestroyWindow(window);
	}

	// window getters
	glm::ivec2 GLFWOpenGLContext::getWindowSize() {
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return glm::ivec2(w, h);
	}

	bool GLFWOpenGLContext::getWindowOpen() {
		return !glfwWindowShouldClose(window);
	}

	// rendering
	void GLFWOpenGLContext::FrameStart() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GLFWOpenGLContext::FrameEnd() {
		glm::ivec2 ws = getWindowSize();

		ImGui::Render();
		glViewport(0, 0, ws.x, ws.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}