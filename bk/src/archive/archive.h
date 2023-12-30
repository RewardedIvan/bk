#pragma once
// useful for shaders, 3d models, ui, textures...

// yolo zip lul
namespace bk::archive {
	struct File {
		uint8_t fnSize;
		char* fn = nullptr;
		uint32_t dataCRC32;
		uint64_t dataSize;
		char* data = nullptr;

		const std::string getName();
		const std::string getData();

		void saveToFile(fs::path location) const;
		void rename(const std::string& newName);

		File(fs::path path);
		File(const std::vector<char>& data);
		~File();
		File();
		const std::vector<char> serialize() const;
	};

	struct Archive {
		static const uint16_t LATEST_VER = 0x2;

		uint16_t version = LATEST_VER;
		uint64_t fileCount = 0;
		std::vector<uint64_t> fileSizes;
		std::vector<File*> files;

		void addFile(File* f);
		void removeFile(File* f);
		void removeFile(int64_t index);
		File* findFile(const char* name);
		std::vector<File*> findFiles(const char* name);

		Archive(fs::path path);
		Archive();
		~Archive();
		void saveToFile(fs::path path);
	};
}
