#ifndef CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C
#define CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C

#include <tc/parser/reader/Reader.h>
#include <tc/common/Result.h>
#include <tc/common/Logger.h>

namespace tc::parser::records {

class RecordI : public common::LogI {
public:
  RecordI() : common::LogI("console") {}
  virtual ~RecordI() = default;

  virtual result_t parse(const reader::ReaderSPtr &reader, int codec) = 0;
  virtual result_t parse(const reader::ReaderSPtr &reader) = 0;

	virtual bool empty() const = 0;
	virtual std::string toString() = 0;
};

} // namespace tc::parser::records

#endif /* CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C */
