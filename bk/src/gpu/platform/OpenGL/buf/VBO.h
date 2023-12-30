#pragma once

#include "../bindable.h"

namespace bk::gpu::opengl {
	struct LayoutVariable {
		GLenum type;
		uint32_t typeSize;
		uint32_t count; // variables
		GLenum normalized;
	};

	struct VBO : bindable {
		VBO(const void* data, uint32_t size);
		~VBO();

		virtual void bind() const override;
		virtual void unbind() const override;

		template <typename T>
		void add(uint32_t count);

		const std::vector<LayoutVariable>& getLayout() const;
		const uint32_t& getStride() const;
	private:
		std::vector<LayoutVariable> layout = std::vector<LayoutVariable>();
		uint32_t stride = 0, size = 0;
	};

#define defAdd(cpptype, gltype, normalized) \
	template<> \
	inline void VBO::add<cpptype>(uint32_t count) { \
		layout.push_back(LayoutVariable(gltype, sizeof(cpptype), count, normalized)); \
		stride += sizeof(cpptype) * count; \
	}

	defAdd(float,		GL_FLOAT,			GL_FALSE)
	defAdd(uint32_t,	GL_UNSIGNED_INT,	GL_FALSE)
	defAdd(uint8_t,		GL_UNSIGNED_BYTE,	GL_TRUE	)
}
