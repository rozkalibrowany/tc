#ifndef EA08E4E5_5129_433F_9DA9_28685085CB1C
#define EA08E4E5_5129_433F_9DA9_28685085CB1C

#include <tc/common/Buf.h>
#include <memory>
#include <vector>

namespace tc::parser {

class Reader {
public:
	Reader(const common::Buf &buf, int offset = 0);
	Reader(Reader &&reader) = default;
	Reader(const Reader &reader) = delete;
	~Reader() = default;

	const common::Buf& buf() const;
	int offset() const;
	size_t size() const;

	long readL(int bytes, int offset = 0);
	uint readU(int bytes, int offset = 0);
	int read(int bytes, int offset = 0);
	common::Buf readS(int bytes, int offset = 0);

	void skip(int bytes);
	void setOffset(int offset);

protected:
	common::Buf iBuf;
	mutable int iOffset;
};

} // namespace tc::parser

#endif /* EA08E4E5_5129_433F_9DA9_28685085CB1C */
