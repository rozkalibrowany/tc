#ifndef AF7F8597_2B7B_448F_A38D_4054C93EFC41
#define AF7F8597_2B7B_448F_A38D_4054C93EFC41

#include <tc/parser/Reader.h>
#include <tc/parser/omni/packet/Payload.h>
#include <tc/common/Common.h>
#include <tc/parser/Json.h>
#include <tc/parser/omni/Datetime.h>

namespace tc::parser::omni::records {

class RecordI : public Reader, public tc::LogI {
public:
	static constexpr std::string_view c_response = "Re";

	RecordI(const common::Buf &buf) : Reader(buf), tc::LogI("console"){}
	RecordI(const RecordI &rhs) = default;
	RecordI(RecordI &&rhs) = default;
	virtual ~RecordI() = default;

	virtual result_t response(common::Buf& response) = 0;
	virtual result_t parse(uint8_t elements) = 0;

	virtual result_t parse() {
		LG_NFO(this->logger(), "toHex: {}", bytes2hex((const uint8_t*)iBuf.cdata(), iBuf.size()));

		if (!Payload::valid(iBuf.cdata(), iBuf.size())) {
			LG_NFO(this->logger(), "Invalid data");
			return RES_NOENT;
		}

		Imei imei;
		if (Payload::parseImei(iBuf, imei) != RES_OK) {
			LG_NFO(this->logger(), "Unable to parse Imei");
			return RES_NOENT;
		}

		std::string manufacturer;
		if (Payload::parseManufacturer(iBuf, manufacturer) != RES_OK) {
			LG_NFO(this->logger(), "Unable to parse manufacturer");
			return RES_NOENT;
		}

		Datetime datetime;
		if (Payload::parseDatetime(iBuf, datetime) != RES_OK) {
			LG_NFO(this->logger(), "Unable to parse datetime");
			return RES_NOENT;
		}

		iImei = std::move(imei);
		iManufacturer = std::move(manufacturer);
		iDateTime = datetime;
		return RES_OK;
	}

protected:
	Imei iImei;
	std::string iManufacturer;
	Datetime iDateTime;
};

} // namespace tc::parser::omni::records

#endif /* AF7F8597_2B7B_448F_A38D_4054C93EFC41 */
