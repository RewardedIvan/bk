#pragma once
#include "gpu/Context.h"
#include "Keys.h"

namespace bk::input {
	// mouse
	glm::vec2 getMousePos(gpu::Context* w);
	KeyState getMouseBtn(gpu::Context* w, Key k);

	// keys
	KeyState getKey(gpu::Context* w, Key k);
	std::string keyName(Key k);

	bool isKeyPressed(gpu::Context* w, Key k);
	bool isKeyDown(gpu::Context* w, Key k);
	bool isKeyReleased(gpu::Context* w, Key k);
}
