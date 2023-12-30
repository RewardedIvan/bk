#include "pch/pch.h"

#include "Input.h"
#include "events/GLFWInput.h"
#include "utils/log.h"

#ifdef BK_INCLUDE_GLFW
#include <GLFW/glfw3.h>
#endif

namespace bk::input {
#ifdef BK_INCLUDE_GLFW
	glm::vec2 getMousePos(const GWindow& w) {
		double x, y;
		glfwGetCursorPos(w.getGLFWContext()->window, &x, &y);
		return glm::vec2(x, y);
	}

	static auto mbStates = std::map<Key, KeyState>();
	KeyState getMouseBtn(const GWindow& w, Key btn) {
		if (!mbStates.contains(btn))
			mbStates[btn] = KeyState::RELEASED;

		mbStates[btn] = transitionKeyState(mbStates[btn], getKeyStateFromGLFW(glfwGetMouseButton(w.getGLFWContext()->window, btn)));
		return mbStates[btn];
	}

	static auto keyStates = std::map<Key, KeyState>();
	KeyState getKey(const GWindow& w, Key k) {
		if (!keyStates.contains(k))
			keyStates[k] = KeyState::RELEASED;

		keyStates[k] = transitionKeyState(keyStates[k], getKeyStateFromGLFW(glfwGetKey(w.getGLFWContext()->window, k)));
		return keyStates[k];
	}

	std::string keyName(Key k) {
		return std::string(glfwGetKeyName(k, 0));
	}

	bool isKeyPressed(const GWindow& w, Key k) {
		return getKey(w, k) == KeyState::PRESS;
	}

	bool isKeyDown(const GWindow& w, Key k) {
		return getKey(w, k) == KeyState::DOWN;
	}

	bool isKeyReleased(const GWindow& w, Key k) {
		return getKey(w, k) == KeyState::RELEASED;
	}
#endif
}
