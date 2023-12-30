#pragma once

#include <string_view>

namespace bk {
	struct ANSIColors {
#define ce static constexpr std::string_view
		// stolen from https://stackoverflow.com/a/41407246
		// maybe add `ce CSI = \x1b`, but i dont really want to do this every time `{ANSIColors.CSI}{ANSIColors.Reset}`
		ce Reset = "\x1b[0m";
		ce Bright = "\x1b[1m";
		ce Dim = "\x1b[2m";
		ce Underscore = "\x1b[4m";
		ce Blink = "\x1b[5m";
		ce Reverse = "\x1b[7m";
		ce Hidden = "\x1b[8m";

		ce FgBlack = "\x1b[30m";
		ce FgRed = "\x1b[31m";
		ce FgGreen = "\x1b[32m";
		ce FgYellow = "\x1b[33m";
		ce FgBlue = "\x1b[34m";
		ce FgMagenta = "\x1b[35m";
		ce FgCyan = "\x1b[36m";
		ce FgWhite = "\x1b[37m";
		ce FgGray = "\x1b[90m";
		ce FgBrightRed = "\x1b[91m";
		ce FgBrightGreen = "\x1b[92m";
		ce FgBrightYellow = "\x1b[93m";
		ce FgBrightBlue = "\x1b[94m";
		ce FgBrightMagenta = "\x1b[95m";
		ce FgBrightCyan = "\x1b[96m";
		ce FgBrightWhite = "\x1b[97m";

		ce BgBlack = "\x1b[40m";
		ce BgRed = "\x1b[41m";
		ce BgGreen = "\x1b[42m";
		ce BgYellow = "\x1b[43m";
		ce BgBlue = "\x1b[44m";
		ce BgMagenta = "\x1b[45m";
		ce BgCyan = "\x1b[46m";
		ce BgWhite = "\x1b[47m";
		ce BgGray = "\x1b[100m";
		ce BgBrightRed = "\x1b[101m";
		ce BgBrightGreen = "\x1b[102m";
		ce BgBrightYellow = "\x1b[103m";
		ce BgBrightBlue = "\x1b[104m";
		ce BgBrightMagenta = "\x1b[105m";
		ce BgBrightCyan = "\x1b[106m";
		ce BgBrightWhite = "\x1b[107m";
#undef ce
	};

	struct colutils {
		static std::string color(const char* str, std::string_view col);
	};
}
