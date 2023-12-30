#include "pch/pch.h"

#include "colutils.h"
#include "strutils.h"

namespace bk {
	std::string colutils::color(const char* str, std::string_view col) {
        return strutils::surround(col.data(), str, ANSIColors::Reset.data());
	}
}