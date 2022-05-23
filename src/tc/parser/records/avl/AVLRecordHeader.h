#ifndef D876042F_EF7D_45D4_82F7_B53125967A96
#define D876042F_EF7D_45D4_82F7_B53125967A96

#include <tc/parser/records/RecordI.h>

namespace tc::parser::records::avl {

class AVLRecordHeader : public RecordI {
public:

	AVLRecordHeader(int64_t timestamp, int priority);
	AVLRecordHeader();

	virtual ~AVLRecordHeader() = default;

	AVLRecordHeader &operator=(const AVLRecordHeader &rhs);

	bool empty() const override;
	void clear() override;
	result_t parse(const std::shared_ptr<Reader> &reader) override;
	result_t parse(const std::shared_ptr< Reader > &reader, int codec) override;
	std::string toString() override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	int64_t iTimestamp;
	int iPriority;
};

} // namespace tc::parser::records::avl

#endif /* D876042F_EF7D_45D4_82F7_B53125967A96 */
