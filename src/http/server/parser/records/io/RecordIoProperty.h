#ifndef F2773C62_B1C9_43E7_8B0A_2B8A558E3055
#define F2773C62_B1C9_43E7_8B0A_2B8A558E3055

#include <stdint.h>
#include <vector>

namespace parser::records::io {

class RecordIoProperty;
using RecordIoPropertySptr = std::shared_ptr< RecordIoProperty >;
using RecordsIoPropertyList = std::vector< RecordIoPropertySptr >;

class RecordIoProperty {
public:

  RecordIoProperty(int id, int64_t val) : iID(id), iValue(val) {}
  RecordIoProperty() : RecordIoProperty(0, 0LL) {}

  int iID;
  int64_t iValue;
};

} // namespace parser::records::io

#endif /* F2773C62_B1C9_43E7_8B0A_2B8A558E3055 */
