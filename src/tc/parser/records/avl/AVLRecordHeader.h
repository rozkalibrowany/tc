#ifndef D876042F_EF7D_45D4_82F7_B53125967A96
#define D876042F_EF7D_45D4_82F7_B53125967A96

#include <tc/parser/records/RecordI.h>
#include <chrono>

namespace tc::parser::records::avl {

class AVLRecordHeader : public RecordI {
public:

	AVLRecordHeader(int64_t timestamp, int priority);
	AVLRecordHeader();

	virtual ~AVLRecordHeader() = default;

	AVLRecordHeader &operator=(const AVLRecordHeader &rhs);

	result_t parse(const reader::ReaderSPtr &reader) override;
	result_t parse(const reader::ReaderSPtr &reader, int codec) override;
	std::string toString() override;

protected:
	int64_t iTimestamp;
	int iPriority;
};

} // namespace tc::parser::records::avl

#endif /* D876042F_EF7D_45D4_82F7_B53125967A96 */
