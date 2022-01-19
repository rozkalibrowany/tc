#include <tc/parser/Packet.h>
#include <tc/parser/records/avl/AVLRecord.h>
#include <tc/common/Convert.h>
#include <algorithm>

namespace tc::parser {

size_t Packet::PACKET_DATA_MIN_SIZE = 45;
size_t Packet::PACKET_IMEI_MIN_SIZE = 15;

Packet::Packet(const ID &id)
	: common::LogI("console")
	, iID(id)
{
	// nothing to do
}

Packet::Packet(bool time_now)
 : Packet(ID(time_now))
{
 // nothing to do
}

result_t Packet::parse(Buf &buf)
{
	if (buf.size() < Packet::PACKET_DATA_MIN_SIZE) {
		return RES_NOENT;
	}

	if (contains(buf, '8') || contains(buf, '\x8E')) {
		int offset = -1;

		if (getIdx(buf, '\x8E') == 23 || getIdx(buf, '\x8E') == 8) {
			offset = getIdx(buf,'\x8E');
		} else if (getIdx(buf, '8') == 23 || getIdx(buf, '8') == 8) {
			offset = getIdx(buf, '8');
		}

		auto reader = reader::msptr(buf, offset);
		int codec = reader->readU(1);
		int records_total = reader->readU(1);

		for (int i = 0; i < records_total; i++) {
			auto record = std::make_shared< AVLRecord >(codec);
			if (record->read(reader) != RES_OK)
				break;
			iAVLRecords.add(std::move(record));
		}

		if (records_total != (int) iAVLRecords.size()) {
			return RES_NOENT;
		}

		iCodec = codec;
		iRecordsTotal = records_total;
		iReader = std::move(reader);
		return RES_OK;
	}

	SPDLOG_LOGGER_WARN(this->logger(), "Unknown protocol.");
	return RES_NOENT;
}

result_t Packet::parse(unsigned char* cbuf, size_t size)
{
	Buf buffer(cbuf, size);

	if (size > PACKET_DATA_MIN_SIZE) {
		SPDLOG_LOGGER_INFO(this->logger(), "Parse data");
		return parse(buffer);
	}

	SPDLOG_LOGGER_INFO(this->logger(), "Parse imei");
	return parseImei(buffer);
}

Packet::ID &Packet::id()
{
	return iID;
}

std::string Packet::imei() const
{
	return iImei;
}

int Packet::codec() const
{
	return iCodec;
}

int Packet::size() const
{
	return iRecordsTotal;
}

ReaderSPtr &Packet::reader()
{
	return iReader;
}

AVLRecords &Packet::records()
{
	return iAVLRecords;
}

SysTime Packet::timestamp() const
{
	return iID.timestamp;
}

bool Packet::contains(const Buf &buf, unsigned char c) const
{
	if (std::find(buf.cbegin(), buf.cend(), c) != buf.cend())
			return true;
	return false;
}

int Packet::getIdx(const Buf &buf, const unsigned char c) {
    for (int i = 0; i < 24; i++) {
        if (c == buf[i]) {
            return i;
        }
    }
    return -1;
}

result_t Packet::parseImei(Buf &buf)
{
	if (buf.size() <= 2) {
		return RES_INVARG;
	}

	auto imei_length = ((buf[0]) << 8) | ((buf[1]) << 0);

	buf.iBuf.erase(buf.begin(), buf.begin() + 2);

	auto hex_str = unsigned_char_to_string(&buf.iBuf.front(), imei_length);
	auto reader = reader::msptr(buf);
	auto imei = reader->readImei(hex_str);

	SPDLOG_LOGGER_INFO(this->logger(), "Got IMEI: {}", imei);
	iImei = std::move(imei);
	return RES_OK;
}

Packet &Packet::operator=(const Packet &rhs)
{
	return *this;
}

bool Packet::operator<(const Packet &rhs) const
{
	return iID.timestamp.timestamp() < rhs.iID.timestamp.timestamp();
}

bool Packet::operator<=(const Packet &rhs) const
{
	return iID.timestamp.timestamp() <= rhs.iID.timestamp.timestamp();
}

bool Packet::operator>(const Packet &rhs) const
{
	return iID.timestamp.timestamp() > rhs.iID.timestamp.timestamp();
}

bool Packet::operator>=(const Packet &rhs) const
{
	return iID.timestamp.timestamp() >= rhs.iID.timestamp.timestamp();
}

bool Packet::operator==(const Packet &rhs) const
{
	return iID.timestamp.timestamp() == rhs.iID.timestamp.timestamp();
}

} // namespace tc::parser