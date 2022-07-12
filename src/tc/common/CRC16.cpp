#include <tc/common/CRC16.h>

namespace tc::common {

CRC16::CRC16(int offset, int polynom, int preset)
 : iOffset(offset)
 , iPolynom(polynom)
 , iPreset(preset)
{
	 // nothing to do
}

int CRC16::calc(const char *buf, size_t size)
{
	iPolynom &= 0xFFFF;

	std::vector< char > vBuf(buf, buf + (int) size);

	int crc = iPreset;
	for (size_t i = 0; i < vBuf.size(); i++) {
		int data = vBuf[(i + iOffset) % (int) vBuf.size()] & 0xFF;
		crc ^= data;
		for (int j = 0; j < 8; j++) {
			if ((crc & 0x0001) != 0) {
				crc = (crc >> 1) ^ iPolynom;
			} else {
				crc = crc >> 1;
			}
		}
	}
	return crc & 0xFFFF;
}

} // namespace tc::common
