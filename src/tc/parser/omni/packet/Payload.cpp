#include <tc/parser/omni/packet/Payload.h>
#include <tc/parser/omni/records/CheckIn.h>
#include <tc/parser/omni/records/Heartbeat.h>
#include <tc/parser/omni/records/Unlock.h>
#include <tc/parser/omni/records/LockReport.h>
#include <tc/parser/omni/records/Positioning.h>
#include <tc/parser/omni/Command.h>
#include <magic_enum.hpp>

namespace tc::parser::omni {

bool Payload::valid(const uchar* buf, size_t size)
{
	if (size < 1) return false;
	return (uchar)buf[0] == (uchar)OMNI_HEADER;
}

const size_t Payload::size()
{
	return iSize;
}

const Imei &Payload::imei() const
{
	return iImei;
}

result_t Payload::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;
	
	// getting index where manufacturer should start
	auto index = buf.find_nth(',', 1);
	if (!index && index + MANUFACTURER_SIZE > buf.size())
		return RES_NOMEM;

	iManufacturer.reserve(MANUFACTURER_SIZE);
	iManufacturer = std::string((const char*) (buf.cdata() + index), MANUFACTURER_SIZE);

	// getting index where imei should start
	index = buf.find_nth(',', 2);
	if (!index && index + IMEI_SIZE > buf.size())
		return RES_NOMEM;

	iImei.reserve(IMEI_SIZE);
	iImei = std::string((const char*) (buf.cdata() + index), IMEI_SIZE);

	// getting index where datetime should start
	index = buf.find_nth(',', 3);
	if (!index && (index + DATETIME_SIZE > buf.size()))
		return RES_NOMEM;

	iDateTime.reserve(DATETIME_SIZE);
	iDateTime = std::string((const char*) (buf.cdata() + index), DATETIME_SIZE);
	iSize = buf.size();

	return RES_OK;
}

result_t Payload::parse(const common::Buf &buf, action::Locker::Event event)
{
	result_t res = RES_OK;
	if ((res = parse(buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] parse", imei());
		return res;
	}

	switch(event) {
		case action::Locker::eHeartBeat: {
			auto record = std::make_shared<records::Heartbeat>(false);
			if ((res = record->parse(buf)) == RES_OK)
				iRecords.push_back(record);
			break;
		} case action::Locker::eCheckIn: {
			auto record = std::make_shared<records::CheckIn>(false);
			if((res = record->parse(buf)) == RES_OK)
				iRecords.push_back(record);
			break;
		} case action::Locker::eUnlock: {
			auto record = std::make_shared<records::Unlock>(true);
			if((res = record->parse(buf)) == RES_OK)
				iRecords.push_back(record);
			break;
		} case action::Locker::eLockReport: {
			auto record = std::make_shared<records::LockReport>(true);
			if((res = record->parse(buf)) == RES_OK)
				iRecords.push_back(record);
			break;
		} case action::Locker::eAcquisitionPositioning: {
			auto record = std::make_shared<records::Positioning>(true);
			if((res = record->parse(buf)) == RES_OK)
				iRecords.push_back(record);
			break;
		} default:
			res = RES_NOENT;
		}

	return res;
}

result_t Payload::parse(const uchar* cbuf, size_t size, size_t /* offset */)
{
	common::Buf buf(cbuf, size);
	return parse(buf);
}

const std::vector<Payload::SptrRecord> &Payload::records()
{
	return iRecords;
}

result_t Payload::toJsonImpl(Json::Value &rhs, bool root) const
{
	return RES_NOIMPL;
}

result_t Payload::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return RES_NOIMPL;
}

result_t Payload::getResponse(common::Buf &buf)
{
	if (iRecords.back()->response(buf) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to generate response", imei());
	}

	Command command(imei());
	if (command.init(buf, imei(), iDateTime) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to create command", imei());
	}

	buf = command.buf();
	return RES_OK;
}

} // namespace tc::parser::omni

