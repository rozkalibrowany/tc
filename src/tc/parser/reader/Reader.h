#ifndef EA08E4E5_5129_433F_9DA9_28685085CB1C
#define EA08E4E5_5129_433F_9DA9_28685085CB1C

#include <tc/common/Result.h>
#include <tc/parser/reader/Buf.h>
#include <memory>
#include <iostream>
#include <vector>

namespace tc::parser::reader {

class Reader;
using ReaderUPtr = std::unique_ptr< Reader >;
using ReaderSPtr = std::shared_ptr< Reader >;
using ReaderSCPtr = std::shared_ptr< const Reader >;

template < typename... Args >
ReaderSPtr msptr(Args &&... args)
{
	return std::make_shared< ReaderSPtr::element_type >(std::forward< Args >(args)...);
}

class Reader {
public:
	Reader(Buf &buf);
  Reader(Buf &buf, int offset);

	~Reader() = default;

	Reader &operator=(const Reader &reader);

  int64_t readL(int bytes);
  uint readU(int bytes);
  int read(int bytes);
	void skip(int bytes);

	std::string readImei(const std::string &str);

  Buf &iBuf;
  int iOffset;
};

} // namespace tc::parser::reader

#endif /* EA08E4E5_5129_433F_9DA9_28685085CB1C */
