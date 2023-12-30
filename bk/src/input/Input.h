#pragma once
#include "gpu/GWindow.h"
#include "Keys.h"

namespace bk::input {
	glm::vec2 getMousePos(const GWindow& w);
	KeyState getMouseBtn(const GWindow& w, Key btn);

	KeyState getKey(const GWindow& w, Key k);
	std::string keyName(Key k);

	bool isKeyPressed(const GWindow& w, Key k);
	bool isKeyDown(const GWindow& w, Key k);
	bool isKeyReleased(const GWindow& w, Key k);
}
