#ifndef FC291BE9_416C_46EC_AAF6_9BC66B1BC76C
#define FC291BE9_416C_46EC_AAF6_9BC66B1BC76C

#include <records/ParserI.h>
#include <records/avl/AVLRecordHeader.h>

namespace parser::records::avl {

template<typename T>
class AVLRecordHeaderParser : public ParserI< T > {
public:

  AVLRecordHeaderParser(reader::ReaderSPtr &reader) : ParserI<T>(reader) {}
  virtual ~HeaderParser() {}

protected:
  virtual result_t read(std::shared_ptr<T> &record);

};

template<typename T>
result_t AVLRecordHeaderParser<T>::read(std::shared_ptr<T> &record)
{
  if constexpr (!std::is_same_v<T, AVLRecordHeader> || record == nullptr) {
    return RES_NOENT;
  }

  record->iTimestamp = iReader->readL(8);
  record->iPriority = iReader->read(1);

  return RES_OK;
}

} // namespace parser::records::avl

#endif /* FC291BE9_416C_46EC_AAF6_9BC66B1BC76C */
