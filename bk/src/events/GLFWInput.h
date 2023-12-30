#pragma once
#include "events/event.h"

namespace bk::events {
	struct GLFWInput {
		static const EventID id = 0x01;

		GLFWwindow* window;
		int key, scancode, action, mods;
	};
}
