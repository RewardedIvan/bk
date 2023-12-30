#include "pch/pch.h"

#include "gpu/platform/OpenGL/GLFWOpenGLContext.h"
#include "gpu/Overlay.h"

#include "gpu/platform/OpenGL/buf/VAO.h"
#include "gpu/platform/OpenGL/buf/VBO.h"
#include "gpu/platform/OpenGL/buf/IBO.h"
#include "gpu/platform/OpenGL/Renderer.h"

#include "utils/log.h"
#include "utils/hash.h"
#include "utils/strutils.h"

#include "archive/archive.h"

#include "events/event.h"
#include "input/Input.h"

namespace gl = bk::gpu::opengl;
struct OpenGLTest : bk::Overlay {
	gl::Renderer r;
	//bk::archive::Archive* shaders;
	bk::gpu::opengl::GLFWOpenGLContext& win;

	OpenGLTest(bk::gpu::opengl::GLFWOpenGLContext& win) : win(win) {
		r = gl::Renderer();
		//shaders = new bk::archive::Archive("shaders.ar");
	}

	void render() override {
		r.clear();

		static float pos[] = {
			/* vertices */ /* texcoords */
			-0.5f, -0.5f,	0.0f, 0.0f,
			 0.5f, -0.5f,	1.0f, 0.0f,
			 0.5f,  0.5f,	1.0f, 1.0f,
			-0.5f,  0.5f,	0.0f, 1.0f,
		};

		static uint32_t indecies[] = {
			0, 1, 2,
			2, 3, 0,
		};

		static bool init = false;
		static gl::VBO* vb = new gl::VBO(&pos, sizeof(pos));
		static gl::VAO* va = new gl::VAO();

		static gl::IBO* i = new gl::IBO(&indecies, sizeof(indecies), sizeof(uint32_t));
		static gl::Shader* sh;
		static gl::Texture* tex = new gl::Texture("tex.jpg", bk::gpu::PixelFormat::RGBA8);
		if (!init) {
			r.enableBlending();

			vb->add<float>(2);
			vb->add<float>(2);
			va->addVBO(*vb);

			//sh = new gl::Shader(shaders->findFile("base.vert")->getData().c_str(), shaders->findFile("base.frag")->getData().c_str());
			sh = new gl::Shader(bk::strutils::readFile("sh/base.vert").c_str(), bk::strutils::readFile("sh/base.frag").c_str());
			sh->finish();

			init = true;
		}

		tex->bind(0);
		ImGui::Begin("image debug");
		ImGui::Image((void*)(intptr_t)tex->id, ImVec2((float)tex->getSize().x, (float)tex->getSize().y));
		ImGui::End();

		r.Render(*va, *i, *sh);

		sh->USet2f("uresolution", win.getWindowSize());
		sh->USet1f("utime", static_cast<float>(glfwGetTime()));
		sh->USetTex("tex", *tex);

		//r.Flush();
	}
};

struct ArchiveEditor : bk::Overlay {
	bk::archive::Archive* opened = nullptr;
	bk::archive::File* rightClicked = nullptr;
	bk::archive::File* extracting = nullptr;
	std::string* newName = nullptr;
	ImGui::FileBrowser arfb;
	ImGui::FileBrowser addffb;
	ImGui::FileBrowser savearfb = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
	ImGui::FileBrowser extractfb = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
	std::vector<MemoryEditor> fileDataEditors = std::vector<MemoryEditor>();
	std::vector<bk::archive::File*> editing = std::vector<bk::archive::File*>();

	ArchiveEditor() {
		arfb.SetTitle("open archive");
		arfb.SetTypeFilters({ ".ar" });
		savearfb.SetTitle("save archive");
		savearfb.SetTypeFilters({ ".ar" });
		extractfb.SetTitle("extract to");
	}

	void render() override {
		bool openRightClick = false;
		ImGui::Begin("archive editor"); {
			if (opened != nullptr) {
				if (ImGui::Button("add file to ar"))
					addffb.Open();
				ImGui::SameLine();
				if (ImGui::Button("close ar")) {
					fileDataEditors.clear();
					editing.clear();
					rightClicked = nullptr;
					extracting = nullptr;
					opened->~Archive();
					opened = nullptr;
				}
				ImGui::SameLine();
				if (ImGui::Button("save ar"))
					savearfb.Open();
			} else {
				if (ImGui::Button("open ar"))
					arfb.Open();
				ImGui::SameLine();
				if (ImGui::Button("create ar"))
					opened = new bk::archive::Archive();
			}

			if (opened != nullptr) {
				if (ImGui::BeginTable("ar_files", 4, ImGuiTableFlags_Borders)) {
					ImGui::TableSetupColumn("file");
					ImGui::TableSetupColumn("crc32");
					ImGui::TableSetupColumn("size");
					ImGui::TableSetupColumn("pointer");
					ImGui::TableHeadersRow();

					for (bk::archive::File* f : opened->files) {
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						if (ImGui::Selectable(f->getName().c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)) {
							if (std::find(editing.begin(), editing.end(), f) == editing.end()) {
								editing.push_back(f);
								fileDataEditors.push_back(MemoryEditor());
							}
						}
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
							rightClicked = f;
							openRightClick = true;
						}
						ImGui::TableSetColumnIndex(1);
						ImGui::Text("%x", f->dataCRC32);
						ImGui::TableSetColumnIndex(2);
						ImGui::Text("%s", bk::strutils::formatFileSize(f->dataSize).c_str());
						ImGui::TableSetColumnIndex(3);
						ImGui::Text("%x", f);
					}
					ImGui::EndTable();
				}
			}
		} ImGui::End();

		arfb.Display();
		if (arfb.HasSelected()) {
			opened = new bk::archive::Archive(arfb.GetSelected());
			arfb.ClearSelected();
		}

		if (opened != nullptr) addffb.Display();
		if (addffb.HasSelected()) {
			opened->addFile(new bk::archive::File(addffb.GetSelected()));
			addffb.ClearSelected();
		}

		savearfb.Display();
		if (savearfb.HasSelected()) {
			opened->saveToFile(savearfb.GetSelected());
			savearfb.ClearSelected();
		}

		extractfb.Display();
		if (extractfb.HasSelected()) {
			extracting->saveToFile(extractfb.GetSelected());
			extractfb.ClearSelected();
		}

		for (uint32_t i = 0; i < editing.size(); i++) {
			bk::archive::File* f = editing[i];
			if (fileDataEditors[i].Open)
				fileDataEditors[i].DrawWindow(bk::strutils::suffix("data of ", f->getName().c_str()).c_str(), f->data, f->dataSize);
			else {
				fileDataEditors.erase(fileDataEditors.begin() + i);
				editing.erase(editing.begin() + i);

				// reclaim memory
				editing.shrink_to_fit();
				fileDataEditors.shrink_to_fit();
				break;
			}
		}
		
		if (openRightClick) {
			ImGui::OpenPopup("arf_right_click");
			openRightClick = false;
		}

		if (rightClicked != nullptr && ImGui::BeginPopup("arf_right_click")) {
			renderRightClickPopup();
			ImGui::EndPopup();
		}
	}

	void renderRightClickPopup() {
		ImGui::Text("%s", rightClicked->getName().c_str());

		if (ImGui::Selectable("extract", false)) {
			extracting = rightClicked;
			extractfb.SetInputName(rightClicked->getName());
			extractfb.Open();
			rightClicked = nullptr;
		}

		if (ImGui::Selectable("rename", false, ImGuiSelectableFlags_DontClosePopups)) {
			newName = new std::string(rightClicked->getName().c_str());
			ImGui::OpenPopup("arf_rename");
		}

		if (ImGui::Selectable("remove from ar", false)) {
			opened->removeFile(rightClicked);
			rightClicked->~File();
			rightClicked = nullptr;
		}
		
		if (newName != nullptr && ImGui::BeginPopup("arf_rename")) {
			renderRenamePopup();
			ImGui::EndPopup();
		}
	}

	void renderRenamePopup() {
		ImGui::Text("new name for '%s'", rightClicked->getName().c_str());
		ImGui::InputText("##new name", newName);

		if (ImGui::Button("OK")) {
			rightClicked->rename(*newName);
			ImGui::CloseCurrentPopup();
		} ImGui::SameLine();

		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();
	}
};

struct InputTest : bk::Overlay {
	bk::gpu::Context& win;

	InputTest(bk::gpu::Context& w) : win(w) {
	}
	
	void render() override {
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
};

// bk app
int main() {
	bk::Log::setFile("latest.log");
	bk::input::Keys::SetupGLFW();

	bk::gpu::opengl::GLFWOpenGLContext window;
	window.InitializeAPI();
	window.CreateWindow("bk tests");

	// i dont care if this fails
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\JetBrainsMono-Regular.ttf", 20);

	std::vector<bk::Overlay*> overlays { new OpenGLTest(window), new ArchiveEditor(), new InputTest(window) };

	// imgui style
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImGuiStyle& s = ImGui::GetStyle();
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.50f, 0.53f, 1.00f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.45f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.45f, 0.49f, 1.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.49f, 1.00f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.45f, 0.49f, 1.00f, 0.51f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45f, 0.49f, 1.00f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		s.FrameBorderSize = 0;
		s.WindowBorderSize = 1;
		s.TabBarBorderSize = 1;

		s.WindowRounding = 6;
		s.ChildRounding = 6;
		s.FrameRounding = 2;
		s.PopupRounding = 3;
		s.ScrollbarRounding = 4;
		s.GrabRounding = 2;
		s.TabRounding = 2;

		s.WindowTitleAlign.x = 0.5f;
		s.WindowMenuButtonPosition = ImGuiDir_Right;

		s.GrabMinSize = 7;
		s.ScrollbarSize = 10;
	}

	while (window.getWindowOpen()) {
		window.FrameStart();

		for (bk::Overlay* o : overlays)
			o->render();
		ImGui::ShowDemoWindow();

		window.FrameEnd();
	}

	window.UninitializeAPI();
}