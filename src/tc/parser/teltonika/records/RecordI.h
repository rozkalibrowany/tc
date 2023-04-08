#ifndef CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C
#define CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C

#include <tc/parser/Reader.h>
#include <tc/common/Common.h>
#include <tc/parser/Json.h>

namespace tc::parser::teltonika::records {

class RecordI : public tc::LogI, public JsonI {
public:
  RecordI() : tc::LogI("console") {}
  virtual ~RecordI() = default;

  virtual result_t parse(Reader &reader, int codec) = 0;
  virtual result_t parse(Reader &reader) = 0;

	virtual bool empty() const = 0;
};

} // namespace tc::parser::teltonika::records

#endif /* CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C */
