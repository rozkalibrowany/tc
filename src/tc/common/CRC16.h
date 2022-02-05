#ifndef A5667EF5_A157_45B2_8422_B61E617BD8D0
#define A5667EF5_A157_45B2_8422_B61E617BD8D0

#include <tc/common/Result.h>
#include <tc/parser/reader/Buf.h>

namespace tc::common {

class CRC16
{
public:
	CRC16(int offset = 0, int polynom = 0xA001, int preset = 0) : iOffset(offset), iPolynom(polynom), iPreset(preset) {}

	int calc(uchar *buf, size_t size);

private:
	int iOffset;
	int iPolynom;
	int iPreset;
};

int CRC16::calc(uchar *buf, size_t size)
{
	iPolynom &= 0xFFFF;

	std::vector< uchar > vBuf(buf, buf + (int) size);

	int crc = iPreset;
	for (int i = 0; i < (int) vBuf.size(); i++) {
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


#endif /* A5667EF5_A157_45B2_8422_B61E617BD8D0 */
