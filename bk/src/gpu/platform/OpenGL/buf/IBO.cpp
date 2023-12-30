#include "pch/pch.h"

#include "ibo.h"
#include "utils/log.h"

namespace bk::gpu::opengl {
	IBO::IBO(const void* data, uint32_t size, uint32_t elementSize) :
		elementSize(elementSize), elementCount(size / elementSize) {

		GLCheckErr(glGenBuffers(1, &id));
		this->bind();
		GLCheckErr(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void IBO::bind() const {
		GLCheckErr(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	}

	void IBO::unbind() const {
		GLCheckErr(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	uint32_t IBO::getElementCount() const {
		return elementCount;
	}
}
