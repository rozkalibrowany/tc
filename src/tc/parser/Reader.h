#ifndef EA08E4E5_5129_433F_9DA9_28685085CB1C
#define EA08E4E5_5129_433F_9DA9_28685085CB1C

#include <tc/parser/Buf.h>
#include <memory>
#include <iostream>
#include <vector>

namespace tc::parser {

class Reader {
public:
  Reader(std::unique_ptr< Buf > buf, int offset = 0);
	Reader(const Reader &reader) = delete; // delete copy constructor

	~Reader() = default;

	// Reader &operator=(const Reader &reader);

	int64_t readL(int bytes);
  uint readU(int bytes);
  int read(int bytes);
	void skip(int bytes);

	std::string readImei(const std::string &str);

  std::unique_ptr< Buf > iBuf;
  int iOffset;
};

} // namespace tc::parser

#endif /* EA08E4E5_5129_433F_9DA9_28685085CB1C */
