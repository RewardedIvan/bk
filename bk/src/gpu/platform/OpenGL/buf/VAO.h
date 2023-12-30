#pragma once

#include "../bindable.h"
#include "VBO.h"

namespace bk::gpu::opengl {
	struct VAO : bindable {
		VAO();
		~VAO();
		
		virtual void bind() const override;
		virtual void unbind() const override;

		void addVBO(const VBO& vb) const;
	};
}
