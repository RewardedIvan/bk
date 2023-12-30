#include "pch/pch.h"

#include "strutils.h"
#include "colutils.h"
#include "log.h"

namespace bk::strutils {
	std::string vstrprintf(const char* format, va_list va) {
		va_list va2; va_copy(va2, va);
		int size = vsnprintf(nullptr, 0, format, va2);
		va_end(va2);

		char* buf = (char*)malloc(size + 1);
		if (buf == nullptr) return std::string();

		vsprintf(buf, format, va);
		std::string res(buf);
		free(buf);

		return res;
	}

	std::string strprintf(const char* format, ...) {
		va_list va; va_start(va, format);
		std::string res = vstrprintf(format, va);
		va_end(va);

		return res;
	}

	std::string prefix(const char* prefix, const char* str) {
		return strprintf("%s%s", prefix, str);
	}

	std::string suffix(const char* str, const char* sufx) {
		return strprintf("%s%s", str, sufx);
	}

	std::string surround(const char* prfx, const char* str, const char* sufx) {
		return suffix(prefix(prfx, str).c_str(), sufx);
	}

	// credit: https://stackoverflow.com/a/874160
	bool endsWith(const std::string& str, const std::string& sufx) {
		if (str.length() >= sufx.length())
			return (0 == str.compare(str.length() - sufx.length(), sufx.length(), sufx));
		else
			return false;
	}

	std::string formatFileSize(uint64_t bytes) {
		std::string res;
		if (bytes > 1000000000000)
			res = strprintf("%.2ftb", static_cast<float>(bytes) / 1000000000000);
		if (bytes < 1000000000000)
			res = strprintf("%.2fgb", static_cast<float>(bytes) / 1000000000);
		if (bytes < 1000000000)
			res = strprintf("%.2fmb", static_cast<float>(bytes) / 1000000);
		if (bytes < 1000000)
			res = strprintf("%.2fkb", static_cast<float>(bytes) / 1000);
		if (bytes < 1000)
			res = strprintf("%lub", bytes);

		return res;
	}

	std::string readFile(fs::path path) {
		std::ifstream f(path);
		if (!f.is_open()) {
			Log::error("Couldn't open file '%s'", path.filename().string().c_str());
			return "";
		}

		f.seekg(0, std::ios::end);
		std::streamsize fsize = f.tellg();
		f.seekg(0, std::ios::beg);

		char* buf = (char*)malloc(fsize);
		if (buf == nullptr) {
			Log::error("Couldn't allocate %lld bytes", fsize);
			return "";
		}
		memset(buf, 0, fsize);

		f.read(buf, fsize);
		std::string file(buf, fsize);

		free(buf);
		return file;
	}
}