#pragma once

// OPTIONS
#define BK_INCLUDE_OPENGL
#define BK_INCLUDE_GLFW
#define BK_INCLUDE_IMGUI
//#define BK_LOG_TRACE
#define BK_LOG_DEBUG
#define BK_LOG_STDOUT

#include <glm/glm.hpp>
#include <stb_image.h>

#ifdef BK_INCLUDE_IMGUI
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_freetype.h>
#include <imgui_memory_editor.h>
#include <imfilebrowser.h>
#endif

#ifdef BK_INCLUDE_GLFW
//#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#endif

#ifdef BK_INCLUDE_OPENGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#ifdef BK_INCLUDE_IMGUI
#include <imgui_impl_opengl3.h>
#endif
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
