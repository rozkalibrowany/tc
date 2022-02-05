#include <tc/parser/packet/PacketPayload.h>
#include <algorithm>

namespace tc::parser {

size_t PacketPayload::DATA_MIN_SIZE = 45;
size_t PacketPayload::IMEI_MIN_SIZE = 15;


PacketPayload::PacketPayload(const ID &id)
 : Packet(id)
 , iReader(nullptr)
{
	// nothing to do
}

PacketPayload::PacketPayload(bool time_now)
 : PacketPayload(ID{time_now})
{
	// nothing to do
}

bool PacketPayload::hasPayload(const uchar* buf, size_t size)
{
	bool isPayload = false;
	if (size < PacketPayload::DATA_MIN_SIZE) {
		return isPayload;
	}

	isPayload |= (contains(buf, size, '8') || contains(buf, size, '\x8E'));

	return isPayload;
}

bool PacketPayload::hasPayloadImei(const uchar* buf, size_t size)
{
	bool isPayload = false;
	if (size < PacketPayload::IMEI_MIN_SIZE) {
		return isPayload;
	}

	isPayload |= (((buf[0]) << 8) | (((buf[1]) << 0) > 0));

	return isPayload;
}

bool PacketPayload::contains(const uchar* cbuf, size_t size, uchar c)
{
	auto end = cbuf + (unsigned long) size;
	auto pos = std::find(cbuf, end, c);

	return pos != end;
}

int PacketPayload::getIdx(uchar* cbuf, size_t size, const uchar c) {
	if (size < 24) return -1;

	for (int i = 0; i < 24; i++) {
		if (c == cbuf[i]) {
			return i;
		}
	}
	return -1;
}

result_t PacketPayload::parse(uchar* cbuf, size_t size)
{
	if (size < PacketPayload::IMEI_MIN_SIZE) {
		return RES_NOENT;
	}

	if (contains(cbuf, size, '8') || contains(cbuf, size, '\x8E')) {
		int offset = -1;

		if (getIdx(cbuf, size, '\x8E') == 23 || getIdx(cbuf, size, '\x8E') == 8) {
			offset = getIdx(cbuf, size, '\x8E');
		} else if (getIdx(cbuf, size, '8') == 23 || getIdx(cbuf, size, '8') == 8) {
			offset = getIdx(cbuf, size, '8');
		}

		auto reader = std::make_shared< Reader >(std::make_unique< Buf >(cbuf, size), offset);
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
		iTotalRecords = records_total;
		iReader = std::move(reader);
		return RES_OK;
	}

	LG_WRN(this->logger(), "Unknown protocol.");
	return RES_NOENT;
}

std::string PacketPayload::imei() const
{
	return iImei;
}

std::shared_ptr< Reader > &PacketPayload::reader()
{
	return iReader;
}

AVLRecords &PacketPayload::records()
{
	return iAVLRecords;
}

result_t PacketPayload::parseImei(uchar* cbuf, size_t size)
{
	if (size <= 2) {
		return RES_INVARG;
	}

	auto len = ((cbuf[0]) << 8) | ((cbuf[1]) << 0);
	auto buf = cbuf + 2;

	auto hex_str = tc::unsigned_char_to_string(buf, len);
	auto reader = std::make_shared< Reader >(std::make_unique< Buf >(buf, len));
	auto imei = reader->readImei(hex_str);

	LG_NFO(this->logger(), "Got IMEI: {}", imei);
	iImei = std::move(imei);
	return RES_OK;
}

} // namespace tc::parser