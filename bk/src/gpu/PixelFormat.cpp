#include "pch/pch.h"

#include "PixelFormat.h"

namespace bk::gpu {
	static std::map<PixelFormat, PixelFormat_t> pixelFormats {
		/*Pixel Format			GLenum		bpp	 channels*/
		{ PixelFormat::RGB,		{ GL_RGB,	 8,		3 } },
		{ PixelFormat::RGBA,	{ GL_RGBA,	 8,		4 } },
		{ PixelFormat::BGR,		{ GL_BGR,	 8,		3 } },
		{ PixelFormat::BGRA,	{ GL_BGRA,	 8,		4 } },
	};

	const PixelFormat_t& queryPFData(const PixelFormat& pf) {
		for (auto const& [key, val] : pixelFormats) {
			if (key == pf)
				return val;
		}
		return invalidPF;
	}
}
