#ifndef F2B3EF93_9529_48E8_9906_FDF0C5C0AABD
#define F2B3EF93_9529_48E8_9906_FDF0C5C0AABD

#include <tc/parser/Packet.h>
#include <tc/parser/Json.h>
#include <tc/common/Buf.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/records/RecordI.h>

namespace tc::parser::omni {

#define OMNI_HEADER 0x2a // '*'

class Payload : public Packet
{
	static constexpr size_t IMEI_SIZE         = 15;
	static constexpr size_t DATETIME_SIZE     = 12;
	static constexpr size_t MANUFACTURER_SIZE = 2;

	using SptrRecord = std::shared_ptr<records::RecordI>;

public:

	Payload() = default;
	Payload(const Imei &imei) : Packet(imei) {}
	Payload(const Payload &rhs) = default;
	Payload(Payload &&rhs) = default;

	static bool valid(const uchar* buf, size_t size);

	virtual result_t parse(const common::Buf &buf);
	virtual result_t parse(const common::Buf &buf, const action::Locker::Event event);
	virtual result_t parse(const uchar *cbuf, size_t size, size_t offset = 0) override;

	virtual const Imei &imei() const;
	const size_t size() override;
	const std::vector<SptrRecord> &records();
	result_t getResponse(common::Buf &buf);

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

	size_t iSize {0};
	std::string iManufacturer {"OM"};
	SysTime iDateTime{true};
	std::vector<SptrRecord> iRecords;
};

} // namespace tc::parser::omni

#endif /* F2B3EF93_9529_48E8_9906_FDF0C5C0AABD */
