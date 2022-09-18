#include <tc/parser/packet/PacketPayload.h>
#include <algorithm>

namespace tc::parser {

size_t PacketPayload::DATA_MIN_SIZE = 45;
size_t PacketPayload::IMEI_MIN_SIZE = 15;

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
	if (size < PacketPayload::IMEI_MIN_SIZE || size >= PacketPayload::DATA_MIN_SIZE) {
		return isPayload;
	}

	isPayload |= (((buf[0]) << 8) | (((buf[1]) << 0) > 0)) > 0 ? true : false;

	return isPayload;
}

bool PacketPayload::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

int PacketPayload::getIdx(const uchar* cbuf, size_t size, const uchar c) {
	if (size < 24) return -1;

	for (int i = 0; i < 24; i++) {
		if (c == cbuf[i]) {
			return i;
		}
	}
	return -1;
}

result_t PacketPayload::parse(uchar* cbuf, size_t size, size_t /* offset */)
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
		auto buf = std::make_shared<Buf>(cbuf, size);

		if (crcOk(buf, size) != true) {
			LG_DBG(this->logger(), "Incorrect CRC sum");
			return RES_INVCRC;
		}

		auto reader = std::make_shared<Reader>(std::move(buf), offset);
		auto codec = reader->readU(1);
		auto records_total = reader->readU(1);

		for (uint i = 0; i < records_total; i++) {
			auto record = std::make_shared< AVLRecord >(codec);
			if (record->read(reader) != RES_OK)
				break;
			iAVLRecords.add(std::move(record));
		}

		if (static_cast< size_t >(records_total) != iAVLRecords.size()) {
			LG_WRN(this->logger(), "AVL records[{}] size not equal to read value[{}]", iAVLRecords.size(), records_total);
			return RES_NOENT;
		}

		LG_DBG(this->logger(), "Packet Payload parse. Codec: int {} char {} offset {} size{}", (int)codec, std::to_string(codec), offset, size);

		iRecordsSize = records_total;
		iCodec = codec;
		iReader = reader;
		return RES_OK;
	}

	LG_WRN(this->logger(), "Unknown protocol.");
	return RES_NOENT;
}

const size_t PacketPayload::size()
{
	return iRecordsSize;
}

std::shared_ptr< Reader > PacketPayload::reader()
{
	return iReader;
}

AVLRecords &PacketPayload::records()
{
	return iAVLRecords;
}

result_t PacketPayload::toJsonImpl(Json::Value &rhs, bool root) const
{
	Json::Value val;
	if (result_t res; (res = iAVLRecords.toJson(rhs, root)) != RES_OK) {
		return res;
	}

	return RES_OK;
}

} // namespace tc::parser