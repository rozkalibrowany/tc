#ifndef D876042F_EF7D_45D4_82F7_B53125967A96
#define D876042F_EF7D_45D4_82F7_B53125967A96

#include <records/RecordI.h>
#include <chrono>

namespace parser::records::avl {

class RecordHeader : public RecordI {
public:

	RecordHeader(int64_t timestamp, int priority)
	 : iTimestamp(timestamp)
	 , iPriority(priority) {}

	RecordHeader &operator=(const RecordHeader &rhs) {
		iTimestamp = rhs.iTimestamp;
		iPriority = rhs.iPriority;
		return *this;
	}

	virtual ~RecordHeader() {}

protected:
	virtual std::string toString() {
		return std::string();
	}

	int64_t iTimestamp;
	int iPriority;
};

} // namespace parser::records::avl

#endif /* D876042F_EF7D_45D4_82F7_B53125967A96 */
