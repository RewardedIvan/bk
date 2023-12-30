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
		RGB8 = 0x0,
		RGB16 = 0x1,
		RGB = RGB8,
		RGBA8 = 0x2,
		RGBA16 = 0x3,
		RGBA = RGBA8,
		BGR = 0x4,
		BGRA = 0x5,
	};

	const PixelFormat_t& queryPFData(const PixelFormat& pf);
}
