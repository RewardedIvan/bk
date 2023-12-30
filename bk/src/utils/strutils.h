#pragma once
#include <string>

namespace bk::strutils {
	std::string vstrprintf(const char* format, va_list va);
	std::string strprintf(const char* format, ...);

	std::string prefix(const char* prfx, const char* str);
	std::string suffix(const char* str, const char* sufx);
	std::string surround(const char* prfx, const char* str, const char* sufx);

	bool endsWith(const std::string& str, const std::string& sufx);
	
	std::string formatFileSize(uint64_t bytes);
	std::string readFile(fs::path path);
}
