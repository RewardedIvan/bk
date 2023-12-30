#pragma once

#include "bindable.h"
#include "archive/archive.h"
#include "gpu/PixelFormat.h"

namespace bk::gpu::opengl {
	struct Texture : bindable {
		// needs buf, size, pf initialized
		void OpenGL();
		Texture(fs::path path, PixelFormat pf);
		~Texture();
		Texture(archive::Archive ar, const char* path, PixelFormat pf);

		void bind(uint8_t slot);
		virtual void bind() const override;
		virtual void unbind() const override;

		const glm::uvec2& getSize();
		const uint8_t& getBoundSlot() const;
	private:
		uint8_t slotBoundTo;
		PixelFormat pf;
		void* buf;
		glm::uvec2 size;
	};
}
