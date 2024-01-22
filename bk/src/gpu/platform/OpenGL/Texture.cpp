#include "pch/pch.h"

#include "Texture.h"
#include "utils/log.h"

namespace bk::gpu::opengl {
	void Texture::Upload() {
		if (id == 0)
			GLCheckErr(glGenTextures(1, &id));
		this->bind();

		GLCheckErr(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilteringAlgo2GL(af)));
		GLCheckErr(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilteringAlgo2GL(af)));
		GLCheckErr(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCheckErr(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		PixelFormat_t pfd = queryPFData(pf);
		GLCheckErr(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, pfd.gl, (pfd.bpp == 8) ? GL_UNSIGNED_BYTE : (pfd.bpp == 16) ? GL_UNSIGNED_SHORT : 0, buf));
		
		this->unbind();
	}

	Texture::Texture(fs::path path, PixelFormat pf, TextureFilteringAlgo af) : pf(pf), af(af) {
		if (!fs::exists(path)) {
			Log::error("Texture file doesn't exist!");
			createErrorTexture(); return;
		}

		int w = 0, h = 0;
		int channels;

		stbi_set_flip_vertically_on_load(1);
		this->buf = stbi_load(path.string().c_str(), &w, &h, &channels, queryPFData(pf).channels);
		stbi_set_flip_vertically_on_load(0);
		this->size = glm::uvec2(w, h);

		Upload();
	}

	// intelisense is shit
	Texture::Texture(archive::Archive ar, const char* path, PixelFormat pf, TextureFilteringAlgo af) : pf(pf), af(af) {
		archive::File* f = ar.findFile(path);
		if (f == nullptr) {
			Log::error("Couldn't find texture inside archive! '%s'", path);
			createErrorTexture(); return;
		}

		int w = 0, h = 0;
		int channels;

		stbi_set_flip_vertically_on_load(1);
		this->buf = stbi_load_from_memory((const stbi_uc*)f->data, (int)f->dataSize, &w, &h, &channels, queryPFData(pf).channels);
		stbi_set_flip_vertically_on_load(0);
		this->size = glm::uvec2(w, h);

		Upload();
	}

	void Texture::createErrorTexture(glm::uvec2 size) {
		this->size = size;
		this->buf = malloc((size.y * size.x) * sizeof(uint32_t));
		this->pf = PixelFormat::RGBA;
		this->af = TextureFilteringAlgo::NEAREST;

		for (uint32_t y = 0; y < size.y; y++) {
			for (uint32_t x = 0; x < size.x; x++) {
				if ((x+y) % 2 == 0)
					((uint32_t*)buf)[(y * size.y) + x] = 0xFFFF00FF;
				else
					((uint32_t*)buf)[(y * size.y) + x] = 0xFF000000;
			}
		}

		Upload();
	}

	Texture::~Texture() {
		GLCheckErr(glDeleteTextures(1, &id));
		free(buf);
	}

	void Texture::Activate(uint8_t slot) {
		if (slot < 32) {
			this->slotBoundTo = slot;
			GLCheckErr(glActiveTexture(GL_TEXTURE0 + slot));
		} else
			Log::error("Slot %u is more than 31", slot);
		this->bind();
	}

	void Texture::bind() const {
		GLCheckErr(glBindTexture(GL_TEXTURE_2D, id));
	}

	void Texture::unbind() const {
		GLCheckErr(glBindTexture(GL_TEXTURE_2D, 0));
	}

	const glm::uvec2& Texture::getSize() {
		return size;
	}

	const uint8_t& Texture::getBoundSlot() const {
		return slotBoundTo;
	}
}
