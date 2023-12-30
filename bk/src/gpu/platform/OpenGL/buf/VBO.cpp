#include "pch/pch.h"

#include "utils/log.h"
#include "VBO.h"

namespace bk::gpu::opengl {
	VBO::VBO(const void* data, uint32_t size) : size(size) {
		GLCheckErr(glGenBuffers(1, &id));
		this->bind();
		GLCheckErr(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VBO::~VBO() {
		GLCheckErr(glDeleteBuffers(1, &id));
	}

	void VBO::bind() const {
		GLCheckErr(glBindBuffer(GL_ARRAY_BUFFER, id));
	}

	void VBO::unbind() const {
		GLCheckErr(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	const std::vector<LayoutVariable>& VBO::getLayout() const {
		return layout;
	}

	const uint32_t& VBO::getStride() const {
		return stride;
	}
}
