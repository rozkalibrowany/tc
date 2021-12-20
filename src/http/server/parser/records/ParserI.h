#ifndef A9DE2E71_D566_4B03_9315_B5C011287206
#define A9DE2E71_D566_4B03_9315_B5C011287206

#include <reader/Reader.h>
#include <common/Result.h>
#include <records/RecordI.h>

namespace parser::records {

template <typename T>
class ParserI {
public:

  ParserI(reader::ReaderSPtr &reader) : iReader(reader) {}

  virtual ~ParserI() {};

  template <typename R>
  result_t parse(std::shared_ptr<R> &record) {
    return static_cast<T*>(this)->parse(record);
  }

  reader::ReaderSPtr iReader;


protected:
  virtual result_t set(const reader::ReaderSPtr &reader) {
    return RES_OK;
  }

};

} // namespace parser::records

#endif /* A9DE2E71_D566_4B03_9315_B5C011287206 */
