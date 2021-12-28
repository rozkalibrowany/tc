#ifndef A9DE2E71_D566_4B03_9315_B5C011287206
#define A9DE2E71_D566_4B03_9315_B5C011287206

#include <reader/Reader.h>
#include <common/Result.h>
#include <records/RecordI.h>

namespace parser::records {

template <typename T>
class ParserI {
public:

  ParserI() = default;
  virtual ~ParserI() = default;

  result_t parse(const reader::ReaderSPtr &reader) {
    return static_cast<T*>(this)->parse(reader);
  }
};

} // namespace parser::records

#endif /* A9DE2E71_D566_4B03_9315_B5C011287206 */
