#pragma once

namespace bk::gpu {
	enum WindowMode {
		Fullscreen,
		Windowed,
		WindowedBorderless,
		WindowedFullscreen = WindowedBorderless, // alias
	};
}
