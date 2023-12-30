#pragma once

namespace bk {
	struct hash {
		static uint32_t crc32_impl(uint32_t initial, uint32_t polynomial, const void* buf, uint64_t len);
		
		// poly: 0xEDB88320
		static uint32_t crc32a(const void* buf, uint64_t len);
		// "Castagnoli", poly: 0x1EDC6F41
		static uint32_t crc32c(const void* buf, uint64_t len);
	private:
		static inline std::map<uint32_t /*poly*/, std::array<uint32_t, 256>> CRCTables = {};
	};
}
