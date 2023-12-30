#pragma once

namespace bk::gpu::opengl {
	struct bindable {
		virtual void unbind() const = 0;
		virtual void bind() const = 0;
		uint32_t id = 0;
	};
}
