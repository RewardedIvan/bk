#pragma once

namespace bk {
	struct Log {
		static void trace(const char* format, ...);
		static void debug(const char* format, ...);
		static void info(const char* format, ...);
		static void warn(const char* format, ...);
		static void error(const char* format, ...);
		static void fatal(const char* format, int ExitCode, ...);

#ifdef BK_INCLUDE_OPENGL
#define GLCheckErr(code) code; bk::Log::trace("[call] %s", #code); bk::Log::logOpenGLErrors(__FILE__, __LINE__);
		static void logOpenGLErrors(const char* file, int line);
#endif

		static void setFile(const char* file);
		static void closeFile();
	private:
		static void log(const char* format, const char* prefix, va_list va);
		inline static std::ofstream* f = nullptr;
	};
}
