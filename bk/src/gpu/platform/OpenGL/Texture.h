#pragma once

#include "bindable.h"
#include "archive/archive.h"

#include "gpu/PixelFormat.h"
#include "gpu/TextureFilteringAlgo.h"

namespace bk::gpu::opengl {
	struct Texture : bindable {
		// needs buf, size, pf & af initialized
		void Upload();
		Texture(fs::path path, PixelFormat pf = PixelFormat::RGBA, TextureFilteringAlgo af = TextureFilteringAlgo::LINEAR);
		Texture(archive::Archive ar, const char* path, PixelFormat pf = PixelFormat::RGBA, TextureFilteringAlgo af = TextureFilteringAlgo::LINEAR);
		void createErrorTexture(glm::uvec2 size = glm::uvec2(32, 32));
		~Texture();

		void Activate(uint8_t slot);
		virtual void bind() const override;
		virtual void unbind() const override;

		const glm::uvec2& getSize();
		const uint8_t& getBoundSlot() const;
	private:
		PixelFormat pf;
		TextureFilteringAlgo af;

		uint8_t slotBoundTo;

		void* buf;
		glm::uvec2 size;
	};
}
