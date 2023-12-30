#pragma once

#define BK_INCLUDE_OPENGL
#define BK_INCLUDE_GLFW
#define BK_LOG_TRACE
#define BK_LOG_STDOUT

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
// GL ES 2.0 + GLSL 100
#define IMGUI_GLSL_VERSION "#version 100"
#elif defined(__APPLE__)
// GL 3.2 + GLSL 150
#define IMGUI_GLSL_VERSION "#version 150"
#else
// GL 3.0 + GLSL 130
#define IMGUI_GLSL_VERSION "#version 130"
#endif

#include <glm/glm.hpp>
#include <stb_image.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_freetype.h>
#include <imgui_memory_editor.h>
#include <imfilebrowser.h>

#ifdef BK_INCLUDE_GLFW
//#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#endif

#ifdef BK_INCLUDE_OPENGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <imgui_impl_opengl3.h>
#endif

// std
#include <vector>
#include <map>
#include <array>
#include <queue>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdarg.h>
#include <string_view>
#include <stdint.h>
#include <regex>
#include <functional>

namespace fs = std::filesystem;
