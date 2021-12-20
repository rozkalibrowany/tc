#ifndef CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C
#define CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C

#include <memory>

namespace parser::records {

class RecordI;
using RecordUPtr = std::unique_ptr< RecordI >;
using RecordSPtr = std::shared_ptr< RecordI >;

class RecordI {
public:
  RecordI() {}
  virtual ~RecordI() {}

protected:
//  virtual result_t getParser(std::shared_ptr<T> &parser) = 0;
  virtual std::string toString() = 0;
};

} // namespace parser::records

#endif /* CDB95BC9_0A6E_4C4B_A858_F0CAB1CFC34C */
