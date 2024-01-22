#include "pch/pch.h"

#include "TextureFilteringAlgo.h"

namespace bk::gpu {
#ifdef BK_INCLUDE_OPENGL
	GLenum TextureFilteringAlgo2GL(TextureFilteringAlgo algo) {
		switch (algo) {
		case TextureFilteringAlgo::LINEAR:
			return GL_LINEAR;
		case TextureFilteringAlgo::NEAREST:
			return GL_NEAREST;
		default:
			return 0;
			break;
		}
	}
#endif
}
