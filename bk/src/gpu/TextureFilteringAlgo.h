#pragma once

namespace bk::gpu {
	enum TextureFilteringAlgo {
		NEAREST = 0,
		NONE = 0,
		LINEAR = 1,
	};

#ifdef BK_INCLUDE_OPENGL
	GLenum TextureFilteringAlgo2GL(TextureFilteringAlgo algo);
#endif
}
