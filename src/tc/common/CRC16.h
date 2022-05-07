#ifndef A5667EF5_A157_45B2_8422_B61E617BD8D0
#define A5667EF5_A157_45B2_8422_B61E617BD8D0

#include <tc/common/Common.h>

namespace tc::common {

class CRC16
{
public:
	CRC16(int offset = 0, int polynom = 0xA001, int preset = 0) : iOffset(offset), iPolynom(polynom), iPreset(preset) {}

	int calc(const char *buf, size_t size);

private:
	int iOffset;
	int iPolynom;
	int iPreset;
};

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

uint16_t crc16arc_bit(uint16_t crc, uchar *mem, size_t len) {
    auto data = mem;
    if (data == NULL)
        return 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (unsigned k = 0; k < 8; k++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
        }
    }
    return crc;
}


} // namespace tc::common


#endif /* A5667EF5_A157_45B2_8422_B61E617BD8D0 */
