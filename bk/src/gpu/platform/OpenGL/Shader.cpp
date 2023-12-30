#include "pch/pch.h"

#include "Shader.h"
#include "utils/log.h"

namespace bk::gpu::opengl {
	GLenum ShaderType2GL(ShaderType st) {
		switch (st) {
		case bk::gpu::opengl::ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case bk::gpu::opengl::ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		case bk::gpu::opengl::ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
		case bk::gpu::opengl::ShaderType::Compute:
			return GL_COMPUTE_SHADER;
		default:
			return 0;
		}
	}

	Shader::Shader() {
		programId = GLCheckErr(glCreateProgram());
		finalized = false;
	}

	Shader::Shader(const char* vertex, const char* fragment) : Shader::Shader() {
		this->push(ShaderType::Vertex, vertex);
		this->push(ShaderType::Fragment, fragment);
	}

	Shader::~Shader() {
		glDeleteProgram(programId);
	}

	void Shader::push(ShaderType type, const char* source) {
		shaders[type] = GLCheckErr(glCreateShader(ShaderType2GL(type)));
		uint32_t shId = shaders[type];
		
		GLCheckErr(glShaderSource(shId, 1, &source, nullptr));
		GLCheckErr(glCompileShader(shId));
		
		// error checking
		int32_t compileRes;
		GLCheckErr(glGetShaderiv(shId, GL_COMPILE_STATUS, &compileRes));
		if (compileRes != GL_TRUE) {
			int32_t compileLogLen;
			GLCheckErr(glGetShaderiv(shId, GL_INFO_LOG_LENGTH, &compileLogLen));

			char* msg = (char*)malloc(compileLogLen);
			GLCheckErr(glGetShaderInfoLog(shId, compileLogLen, &compileLogLen, msg));

			Log::error("couldn't compile shader:\n%s", msg);
			free(msg);

			GLCheckErr(glDeleteShader(shId));
			shaders.erase(type);
			return;
		}
		
		GLCheckErr(glAttachShader(programId, shId));
	}

	void Shader::finish() {
		if (finalized) {
			Log::error("Shader is already finalized.");
			return;
		}

		GLCheckErr(glLinkProgram(programId));
		GLCheckErr(glValidateProgram(programId));
		finalized = true;
	}

	void Shader::bind() const {
		if (!finalized) {
			Log::error("Won't bind shader, because it isn't finalized.");
			return;
		}

		if (glIsProgram(programId) == GL_TRUE) {
			GLCheckErr(glUseProgram(programId));
		} else {
			Log::error("Couldn't bind shader, it is either deleted or not initialized.");
		}
	}

	void Shader::unbind() const {
		GLCheckErr(glUseProgram(0));
	}

	int32_t Shader::ULoc(const char* name) {
		return GLCheckErr(glGetUniformLocation(programId, name));
	}

	void Shader::USet1f(const char* name, float v) {
		GLCheckErr(glUniform1f(ULoc(name), v));
	}

	void Shader::USet2f(const char* name, glm::vec2 v) {
		GLCheckErr(glUniform2f(ULoc(name), v.x, v.y));
	}

	void Shader::USetTex(const char* name, const Texture& tex) {
		GLCheckErr(glUniform1i(ULoc(name), tex.getBoundSlot()));
	}
}
