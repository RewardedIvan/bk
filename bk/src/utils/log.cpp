#include "pch/pch.h"

#include "log.h"
#include "strutils.h"
#include "colutils.h"

namespace bk {
	const static std::regex ansiRegex("\x1b\\[[0-9;]*m");

	void Log::log(const char* format, const char* prefix, va_list va) {
		std::string msg = strutils::vstrprintf(strutils::prefix(prefix, format).c_str(), va);
#ifdef BK_LOG_STDOUT
		printf("%s\n", msg.c_str());
#endif
		if (f != nullptr)
			*f << std::regex_replace(msg, ansiRegex, "") << '\n';
	}

	void Log::trace(const char* format, ...) {
#ifdef BK_LOG_TRACE
		va_list va; va_start(va, format);
		log(format, colutils::color("[/] ", ANSIColors::FgBrightCyan).c_str(), va);
		va_end(va);
#endif
	}

	void Log::debug(const char* format, ...) {
#ifdef BK_LOG_DEBUG
		va_list va; va_start(va, format);
		log(format, colutils::color("[#] ", ANSIColors::FgGray).c_str(), va);
		va_end(va);
#endif
	}

	void Log::info(const char* format, ...) {
		va_list va; va_start(va, format);
		log(format, "[*] ", va);
		va_end(va);
	}

	void Log::warn(const char* format, ...) {
		va_list va; va_start(va, format);
		log(format, colutils::color("[!] ", ANSIColors::FgYellow).c_str(), va);
		va_end(va);
	}

	void Log::error(const char* format, ...) {
		va_list va; va_start(va, format);
		log(format, colutils::color("[%%] ", ANSIColors::FgRed).c_str(), va);
		va_end(va);
	}
	
	void Log::fatal(const char* format, int ExitCode, ...) {
		va_list va; va_start(va, ExitCode);
		log(format, colutils::color("[$] ", ANSIColors::FgBrightRed).c_str(), va);
		va_end(va);

		Log::closeFile();
		exit(ExitCode);
	}

#ifdef BK_INCLUDE_OPENGL
	void Log::logOpenGLErrors(const char* file, int line) {
		while (GLenum err = glGetError()) {
			Log::error("[GL] %s:%d; %d (0x0%2x)", file, line, err, err);
			__debugbreak();
		}
	}
#endif

	void Log::setFile(const char* file) {
		if (f != nullptr) f->close();
		if (fs::exists(file))
			fs::remove(file);

		f = new std::ofstream(file, std::ios::app);
	}

	void Log::closeFile() {
		if (f != nullptr)
			f->close();
	}
}