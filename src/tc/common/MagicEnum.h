#ifndef EF1AC862_7572_4055_B2D7_E7EDAEDCC958
#define EF1AC862_7572_4055_B2D7_E7EDAEDCC958

#include <limits>

/* setting custom range, so we can use custom values in enum
* for example: Omni protocol
* see limitations: https://github.com/Neargye/magic_enum/blob/master/doc/limitations.md
*/

#define MAGIC_ENUM_RANGE_MIN 1
#define MAGIC_ENUM_RANGE_MAX std::numeric_limits<uint16_t>::max() - 1

#include <magic_enum.hpp>

namespace tc {

using namespace magic_enum;

} // namespace tc

#endif /* EF1AC862_7572_4055_B2D7_E7EDAEDCC958 */
