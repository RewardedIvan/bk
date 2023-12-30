#include "pch/pch.h"

#include "GWindow.h"
#include "utils/log.h"
#include "events/GLFWInput.h"

namespace bk {
	GWindow::GWindow(gpu::GPUAPI gapi, const char* name, glm::ivec2 size) {
		switch (gapi) {
		case gpu::GLFWOpenGL: {
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			gpuctx = new gpu::GLFWOpenGLContext(
				glfwCreateWindow(size.x, size.y, name, nullptr, nullptr)
			);
			glfwMakeContextCurrent(getGLFWContext()->window);
			glfwSwapInterval(1); // vsync

			GLenum err = glewInit();
			if (err != GLEW_OK)
				Log::fatal("couldn't initialize glew %d", err);

			Log::info("using GLFW + OpenGL %s", glGetString(GL_VERSION));

			// imgui
			ImGui_ImplGlfw_InitForOpenGL(getGLFWContext()->window, true);
			ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);

			// input
			glfwSetKeyCallback(getGLFWContext()->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				events::GLFWInput* eventData = new events::GLFWInput();
				eventData->window = window;
				eventData->key = key;
				eventData->scancode = scancode;
				eventData->action = action;
				eventData->mods = mods;

				events::EventManager::broadcast(events::GLFWInput::id, eventData);
			});
			break;
		}
		default:
			gpuctx = nullptr;
			break;
		}
	}

	GWindow::~GWindow() {
		switch (getAPI()) {
		case gpu::GLFWOpenGL:
			glfwDestroyWindow(getGLFWContext()->window);
			break;
		default:
			break;
		}
	}

	gpu::GPUAPI GWindow::getAPI() const {
		return gpuctx->getAPI();
	}

	gpu::GLFWOpenGLContext* GWindow::getGLFWContext() const {
		return static_cast<gpu::GLFWOpenGLContext*>(gpuctx);
	}

	void GWindow::RenderStart() {
		switch (getAPI()) {
#ifdef BK_INCLUDE_OPENGL
		case gpu::GPUAPI::GLFWOpenGL:
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glClear(GL_COLOR_BUFFER_BIT);
			break;
#endif
		default:
			break;
		}
	}

	void GWindow::RenderEnd() {
		glm::ivec2 ws = getWindowSize();

		switch (getAPI()) {
#ifdef BK_INCLUDE_OPENGL
		case gpu::GPUAPI::GLFWOpenGL:
			ImGui::Render();
			glViewport(0, 0, ws.x, ws.y);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(getGLFWContext()->window);
			glfwPollEvents();
			break;
#endif
		default:
			break;
		}
	}
	
	bool GWindow::getOpen() const {
		return !glfwWindowShouldClose(getGLFWContext()->window);
	}

	glm::ivec2 GWindow::getWindowSize() {
		switch (getAPI()) {
		case gpu::GPUAPI::GLFWOpenGL:
			int w, h;
			glfwGetFramebufferSize(getGLFWContext()->window, &w, &h);
			return glm::ivec2(w, h);
		default:
			return glm::ivec2(0, 0);
		}
	}
}
