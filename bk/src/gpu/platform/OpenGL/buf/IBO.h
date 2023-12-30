#pragma once

#include "../bindable.h"

namespace bk::gpu::opengl {
	struct IBO : bindable {
		IBO(const void* data, uint32_t size, uint32_t elementSize);

		virtual void bind() const override;
		virtual void unbind() const override;

		uint32_t getElementCount() const;
	private:
		uint32_t elementCount = 0;
		uint32_t elementSize = 0;
	};
}
