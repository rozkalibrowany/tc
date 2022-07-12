#ifndef A5667EF5_A157_45B2_8422_B61E617BD8D0
#define A5667EF5_A157_45B2_8422_B61E617BD8D0

#include <tc/common/Common.h>

namespace tc::common {

class CRC16
{
public:
	CRC16(int offset = 0, int polynom = 0xA001, int preset = 0);

	int calc(const char *buf, size_t size);

private:
	int iOffset;
	int iPolynom;
	int iPreset;
};

} // namespace tc::common

#endif /* A5667EF5_A157_45B2_8422_B61E617BD8D0 */
