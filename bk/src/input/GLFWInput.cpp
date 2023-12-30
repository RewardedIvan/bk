#include "pch/pch.h"

#include "Input.h"
#include "events/GLFWInputEvent.h"
#include "utils/log.h"

#include <GLFW/glfw3.h>
#include "gpu/platform/OpenGL/GLFWOpenGLContext.h"

namespace bk::input {
#define glfwCtx ((gpu::opengl::GLFWOpenGLContext*)w)

	glm::vec2 getMousePos(gpu::Context* w) {
		double x, y;
		glfwGetCursorPos(glfwCtx->window, &x, &y);
		return glm::vec2(x, y);
	}

	// naming things is hard
#define getKeyStateImpl(fnName, fn, mapName) \
	static auto mapName = std::map<Key, KeyState>(); \
	KeyState fnName(gpu::Context* w, Key k) { \
		if (!mapName.contains(k)) \
			mapName[k] = KeyState::RELEASED; \
		mapName[k] = transitionKeyState(mapName[k], getKeyStateFromGLFW(fn(glfwCtx->window, k))); \
		return mapName[k]; \
	}

	getKeyStateImpl(getMouseBtn, glfwGetMouseButton, mbStates)
	getKeyStateImpl(getKey, glfwGetKey, keyStates)

	std::string keyName(Key k) {
		return std::string(glfwGetKeyName(k, 0));
	}

#define checkKeyStateImpl(fnName, state) \
	bool fnName(gpu::Context* w, Key k) { \
		return getKey(w, k) == state; \
	}

	checkKeyStateImpl(isKeyPressed, KeyState::PRESS)
	checkKeyStateImpl(isKeyDown, KeyState::DOWN)
	checkKeyStateImpl(isKeyReleased, KeyState::RELEASED)
}
