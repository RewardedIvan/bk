#include "../pch.h"

#include "input.h"

InputTest::InputTest(bk::gpu::Context& w) : win(w) {
}

void InputTest::render() {
	ImGui::Begin("input"); {
		using k = bk::input::Keys;
		ImGui::Text("key A state: %d", bk::input::getKey(&win, k::K_A));
		ImGui::Text("key '%s' press: %d", bk::input::keyName(k::K_B).c_str(), bk::input::isKeyPressed(&win, k::K_B));
		ImGui::Text("key C down: %d", bk::input::isKeyDown(&win, k::K_C));
		ImGui::Text("alt+C down: %d", bk::input::isKeyDown(&win, k::K_C) && bk::input::isKeyDown(&win, k::K_LEFT_ALT));
		ImGui::Text("mb1 state: %d", bk::input::getMouseBtn(&win, k::K_MB1));
		ImGui::Text("mb2 state: %d", bk::input::getMouseBtn(&win, k::K_MB2));
		ImGui::Text("mb3 state: %d", bk::input::getMouseBtn(&win, k::K_MB3));
	} ImGui::End();
}
