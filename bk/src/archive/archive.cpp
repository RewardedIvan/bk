#include "pch/pch.h"

#include "archive.h"
#include "utils/hash.h"
#include "utils/strutils.h"
#include "utils/log.h"

namespace bk::archive {
#pragma region Serialization Utils
	template<typename T>
	void serializeV(std::vector<char>& buf, const T& data) {
		size_t prevSize = buf.size();
		buf.resize(prevSize + sizeof(T));
		std::memcpy(buf.data() + prevSize, &data, sizeof(T));
	}

	// pointers
	template<typename T>
	void serializeV(std::vector<char>& buf, const T* data, uint64_t size) {
		size_t prevSize = buf.size();
		buf.resize(prevSize + size);
		std::memcpy(buf.data() + prevSize, data, size);
	}

	template<typename T>
	T deserializeV(const std::vector<char>& buf, size_t& offset) {
		T data;
		std::memcpy(&data, buf.data() + offset, sizeof(T));
		offset += sizeof(T);
		return data;
	}

	template<typename T>
	void deserializeV(const std::vector<char>& buf, size_t& offset, T* data, uint64_t size) {
		std::memcpy(data, buf.data() + offset, size);
		offset += size;
	}
#pragma endregion

	const std::vector<char> File::serialize() const {
		std::vector<char> buf;

		serializeV(buf, this->fnSize);
		serializeV(buf, this->fn, this->fnSize);
		serializeV(buf, this->dataCRC32);
		serializeV(buf, this->dataSize);
		serializeV(buf, this->data, this->dataSize);

		return buf;
	}

	File::File(const std::vector<char>& data) {
		uint64_t offset = 0;

		this->fnSize = deserializeV<uint8_t>(data, offset);

		this->fn = new char[this->fnSize];
		deserializeV(data, offset, this->fn, this->fnSize);

		this->dataCRC32 = deserializeV<uint32_t>(data, offset);
		this->dataSize = deserializeV<uint64_t>(data, offset);

		this->data = new char[this->dataSize];
		deserializeV(data, offset, this->data, this->dataSize);
	}

	File::~File() {
		delete[] this->data;
		delete[] this->fn;
	}

	const std::string File::getName() {
		std::string s = std::string(fn, fnSize);
		uint64_t lastSlash = s.find_last_of("/");

		if (lastSlash == std::string::npos)
			return s;

		return s.substr(lastSlash);
	}

	const std::string File::getData() {
		return std::string(this->data, this->dataSize);
	}

	void File::saveToFile(fs::path location) const {
		std::ofstream of(location);
		of.write(data, dataSize);
		of.close();
	}

	void File::rename(const std::string& newName) {
		if (newName.length() > 0xFF) {
			Log::error("New name is longer than path limit (%u/%u)", newName.length(), 0xFF);
			return;
		}

		delete[] fn;
		fn = new char[newName.length()];
		fnSize = static_cast<uint8_t>(newName.length());
		memcpy(fn, newName.c_str(), fnSize);
	}

	File::File(fs::path path) {
		std::ifstream f(path);
		if (!f.is_open())
			Log::fatal("couldn't open file '%s'", 6, path.string().c_str());

		f.seekg(0, std::ios::end);
		std::streamsize fsize = f.tellg();
		f.seekg(0, std::ios::beg);

		if (fsize > 1000000000)
			Log::fatal("not allocating more than a gig", 8);

		char* buf = new char[fsize];
		memset(buf, 0, fsize);
		f.read(buf, fsize);
		//buf[fsize] = '\0'; // add null terminator

		std::string fnS = path.filename().string();
		char* fn = new char[fnS.length()];
		memcpy(fn, fnS.c_str(), fnS.length());

		this->fnSize = static_cast<uint8_t>(fnS.length());
		this->fn = fn;
		this->dataSize = fsize;
		this->dataCRC32 = hash::crc32a(buf, fsize);
		this->data = buf;

		f.close();
	}

#pragma warning(push)
#pragma warning(disable:26495)
	File::File() {
	}
#pragma warning(pop)

	void Archive::addFile(File* f) {
		fileCount++;
		files.push_back(f);
		fileSizes.push_back(f->serialize().size());
	}

	void Archive::removeFile(File* f) {
		auto it = std::find(files.begin(), files.end(), f);
		if (it != files.end())
			removeFile(it - files.begin());
	}

	void Archive::removeFile(int64_t index) {
		files.erase(files.begin() + index);
		fileSizes.erase(fileSizes.begin() + index);
		fileCount--;
	}

	File* Archive::findFile(const char* name) {
		for (File* f : files) {
			if (f->fnSize == strlen(name) && memcmp(name, f->fn, f->fnSize) == 0)
				return f;
		}
		return nullptr;
	}

	std::vector<File*> Archive::findFiles(const char* name) {
		std::vector<File*> found;
		for (File* f : files) {
			if (strutils::endsWith(std::string(f->fn, f->fnSize), std::string(name)))
				found.push_back(f);
		}
		return found;
	}

	void Archive::saveToFile(fs::path path) {
		std::ofstream f(path, std::ios::out);
		std::vector<char> buf;

		serializeV(buf, this->version);
		serializeV(buf, this->fileCount);
		serializeV(buf, this->fileSizes.data(), this->fileSizes.size() * sizeof(uint64_t));
		for (size_t i = 0; i < this->fileCount; i++) {
			std::vector<char> fbuf = files[i]->serialize();

			buf.reserve(buf.size() + fbuf.size()); // reserve memory
			buf.insert(buf.end(), fbuf.begin(), fbuf.end());
		}

		f.write(buf.data(), buf.size());
		f.close();
	}

	Archive::Archive(fs::path path) {
		std::ifstream f(path);
		f.seekg(0, std::ios::end);
		std::streamsize fsize = f.tellg();
		f.seekg(0, std::ios::beg);

		std::vector<char> buf(fsize);
		f.read(buf.data(), fsize);

		uint64_t offset = 0;

		this->version = deserializeV<uint16_t>(buf, offset);
		if (version != LATEST_VER)
			Log::fatal("archive wasnt using the latest version format", 3);

		this->fileCount = deserializeV<uint64_t>(buf, offset);
		this->fileSizes = std::vector<uint64_t>(fileCount);
		this->files = std::vector<File*>();

		deserializeV(buf, offset, fileSizes.data(), fileCount * sizeof(uint64_t));
		for (uint64_t i = 0; i < fileCount; i++) {
			std::vector<char> fl(fileSizes[i]);
			deserializeV(buf, offset, fl.data(), fileSizes[i]);

			File* nf = new File(fl);
			if (nf->dataCRC32 != hash::crc32a(nf->data, nf->dataSize))
				Log::fatal("unable to verify one of the file's crc32; index %lu", 4, i);
			files.push_back(nf);
		}
	}

	Archive::Archive() {
	}
	
	Archive::~Archive() {
		for (archive::File* f : this->files) {
			f->~File();
		}
	}
}
