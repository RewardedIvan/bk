#include "../pch.h"

#include "ArchiveEditor.h"

ArchiveEditor::ArchiveEditor() {
	arfb.SetTitle("open archive");
	arfb.SetTypeFilters({ ".ar" });
	savearfb.SetTitle("save archive");
	savearfb.SetTypeFilters({ ".ar" });
	extractfb.SetTitle("extract to");
}

void ArchiveEditor::render() {
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
		}
		else {
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

void ArchiveEditor::renderRightClickPopup() {
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

void ArchiveEditor::renderRenamePopup() {
	ImGui::Text("new name for '%s'", rightClicked->getName().c_str());
	ImGui::InputText("##new name", newName);

	if (ImGui::Button("OK")) {
		rightClicked->rename(*newName);
		ImGui::CloseCurrentPopup();
	} ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();
}