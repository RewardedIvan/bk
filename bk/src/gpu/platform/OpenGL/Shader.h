#pragma once

#include "bindable.h"
#include "Texture.h"

namespace bk::gpu::opengl {
	enum class ShaderType {
		Vertex,
		Fragment,
		Geometry,	// unused
		Compute,	// unused
	};
	GLenum ShaderType2GL(ShaderType st);

	struct Shader : bindable {
		Shader();
		Shader(const char* vertex, const char* fragment);
		~Shader();

		void push(ShaderType type, const char* source);
		void finish();

		void bind() const;
		void unbind() const;
		
		// uniforms
		int32_t ULoc(const char* name);
		void USet1f(const char* name, float v);
		void USet2f(const char* name, glm::vec2 v);
		void USetTex(const char* name, const Texture& tex);
	private:
		bool finalized;
		std::map<ShaderType, uint32_t/*id*/> shaders;
		uint32_t programId;
	};
}
