#include "pch/pch.h"

#include "Context.h"
#include "utils/log.h"
#include "input/Input.h"

namespace bk::gpu {
	void Context::InitializeAPI() {
#ifdef BK_INCLUDE_IMGUI
		// imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// style
		ImGui::StyleColorsDark();
#endif
	}

	Context::Context(API api) : api(api) {
	}

	API Context::getAPI() const {
		return api;
	}
}