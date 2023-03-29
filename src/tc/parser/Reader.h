#ifndef EA08E4E5_5129_433F_9DA9_28685085CB1C
#define EA08E4E5_5129_433F_9DA9_28685085CB1C

#include <tc/parser/Buf.h>
#include <memory>
#include <vector>

namespace tc::parser {

class Reader {
public:
  Reader(std::shared_ptr<Buf> buf, int offset = 0);
	Reader(const Reader &reader) = default; 
	~Reader() = default;

	std::shared_ptr<Buf> buf();
	int offset() const;

	long readL(int bytes, int offset = 0);
  uint readU(int bytes, int offset = 0);
  int read(int bytes, int offset = 0);
	void skip(int bytes);

private:
	std::shared_ptr<Buf> iBuf;
	mutable int iOffset;
};

} // namespace tc::parser

#endif /* EA08E4E5_5129_433F_9DA9_28685085CB1C */
