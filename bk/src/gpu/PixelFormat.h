#pragma once

namespace bk::gpu {
	struct PixelFormat_t {
#define GLenum uint32_t
		GLenum gl;
#undef GLenum
		uint8_t bpp;
		uint8_t channels;
	};

	static inline PixelFormat_t invalidPF { 0xFFFF };

	enum class PixelFormat {
		RGB = 0x1,
		RGBA = 0x2,
		BGR = 0x3,
		BGRA = 0x4,
	};

	const PixelFormat_t& queryPFData(const PixelFormat& pf);
}
