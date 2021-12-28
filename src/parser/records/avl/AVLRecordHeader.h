#ifndef D876042F_EF7D_45D4_82F7_B53125967A96
#define D876042F_EF7D_45D4_82F7_B53125967A96

#include <records/RecordI.h>
#include <chrono>

namespace parser::records::avl {

class AVLRecordHeader : public RecordI< AVLRecordHeader > {
public:

	AVLRecordHeader(int64_t timestamp, int priority);
	AVLRecordHeader();

	virtual ~AVLRecordHeader() = default;

	AVLRecordHeader &operator=(const AVLRecordHeader &rhs);

	virtual result_t parse(const reader::ReaderSPtr &reader);
	virtual result_t parse(const reader::ReaderSPtr &reader, int codec) { return RES_OK; };
	std::string toString();

protected:
	int64_t iTimestamp;
	int iPriority;
};

} // namespace parser::records::avl

#endif /* D876042F_EF7D_45D4_82F7_B53125967A96 */
