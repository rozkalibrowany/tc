#ifndef D876042F_EF7D_45D4_82F7_B53125967A96
#define D876042F_EF7D_45D4_82F7_B53125967A96

#include <tc/parser/records/RecordI.h>
#include <tc/parser/Reader.h>

namespace tc::parser::records::avl {

class AVLRecordHeader : public RecordI {
public:

	AVLRecordHeader() = default;
	AVLRecordHeader(int64_t timestamp, int priority);
	AVLRecordHeader(AVLRecordHeader&& rhs) = default;

	~AVLRecordHeader() = default;

	AVLRecordHeader &operator=(const AVLRecordHeader &rhs);

	bool empty() const override;
	result_t parse(Reader &reader) override;
	result_t parse(Reader &reader, int codec) override;

	std::optional<long> iTimestamp;
	std::optional<int> iPriority;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

};

} // namespace tc::parser::records::avl

#endif /* D876042F_EF7D_45D4_82F7_B53125967A96 */
