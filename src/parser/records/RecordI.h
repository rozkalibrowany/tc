#ifndef CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C
#define CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C

#include <reader/Reader.h>
#include <common/Result.h>

namespace parser::records {

class RecordI {
public:
  RecordI() = default;
  virtual ~RecordI() = default;

  virtual result_t parse(const reader::ReaderSPtr &reader, int codec) = 0;
  virtual result_t parse(const reader::ReaderSPtr &reader) = 0;

  virtual std::string toString() = 0;
};

} // namespace parser::records

#endif /* CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C */
