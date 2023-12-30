#include "pch/pch.h"

#include "vao.h"
#include "utils/log.h"

namespace bk::gpu::opengl {
	VAO::VAO() {
		GLCheckErr(glGenVertexArrays(1, &id));
	}

	VAO::~VAO() {
		GLCheckErr(glDeleteVertexArrays(1, &id));
	}

	void VAO::bind() const {
		GLCheckErr(glBindVertexArray(id));
	}

	void VAO::unbind() const {
		GLCheckErr(glBindVertexArray(0));
	}

	void VAO::addVBO(const VBO& vb) const {
		bind();
		vb.bind();
		uint32_t i = 0, offset = 0;
		for (const LayoutVariable& var : vb.getLayout()) {
			GLCheckErr(glEnableVertexAttribArray(i));
			GLCheckErr(glVertexAttribPointer(i, var.count, var.type, var.normalized, vb.getStride(), (const void*)(intptr_t)offset));
			i++; offset += var.count * var.typeSize;
		}
	}
}