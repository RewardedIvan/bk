#pragma once

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

	ArchiveEditor();
	void render() override;

	void renderRightClickPopup();
	void renderRenamePopup();
};
