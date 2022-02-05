#ifndef CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C
#define CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C

#include <tc/parser/Reader.h>
#include <tc/common/Common.h>

namespace tc::parser::records {

class RecordI : public tc::LogI {
public:
  RecordI() : tc::LogI("console") {}
  virtual ~RecordI() = default;

  virtual result_t parse(const std::shared_ptr< Reader > &reader, int codec) = 0;
  virtual result_t parse(const std::shared_ptr< Reader > &reader) = 0;

	virtual void clear() = 0;
	virtual bool empty() const = 0;
	virtual std::string toString() = 0;
};

} // namespace tc::parser::records

#endif /* CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C */
