#include <tc/parser/teltonika/packet/Payload.h>
#include <algorithm>

namespace tc::parser::teltonika {

size_t Payload::DATA_MIN_SIZE = 45;
size_t Payload::IMEI_MIN_SIZE = 15;

bool Payload::isTeltonika(const uchar* buf, size_t size)
{
	return (hasPayloadImei(buf, size) || hasPayload(buf, size));
}

bool Payload::hasPayload(const uchar* buf, size_t size)
{
	bool isPayload = false;
	if (size < Payload::DATA_MIN_SIZE) {
		return isPayload;
	}

	isPayload |= (contains(buf, size, '8') || contains(buf, size, '\x8E'));

	return isPayload;
}

bool Payload::hasPayloadImei(const uchar* buf, size_t size)
{
	bool isPayload = false;
	if (size < Payload::IMEI_MIN_SIZE || size >= Payload::DATA_MIN_SIZE) {
		return isPayload;
	}

	isPayload |= (((buf[0]) << 8) | (((buf[1]) << 0) > 0)) > 0 ? true : false;

	return isPayload;
}

bool Payload::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

int Payload::getIdx(const uchar* cbuf, size_t size, const uchar c) {
	if (size < 24) return -1;

	for (int i = 0; i < 24; i++) {
		if (c == cbuf[i]) {
			return i;
		}
	}
	return -1;
}

result_t Payload::parse(const uchar* cbuf, size_t size, size_t /* offset */)
{
	if (size < Payload::IMEI_MIN_SIZE) {
		return RES_NOENT;
	}

	if (contains(cbuf, size, '8') || contains(cbuf, size, '\x8E')) {
		int offset = -1;

		if (getIdx(cbuf, size, '\x8E') == 23 || getIdx(cbuf, size, '\x8E') == 8) {
			offset = getIdx(cbuf, size, '\x8E');
		} else if (getIdx(cbuf, size, '8') == 23 || getIdx(cbuf, size, '8') == 8) {
			offset = getIdx(cbuf, size, '8');
		}
		auto buf = std::make_shared<common::Buf>(cbuf, size);

		if (crcOk(buf, size) != true) {
			LG_DBG(this->logger(), "Incorrect CRC sum");
			return RES_INVCRC;
		}

		Reader reader(buf, offset);
		auto codec = reader.read(1);
		auto records_total = reader.read(1);

		for (int i = 0; i < records_total; i++) {
			AVLRecord record(codec);
			if (record.read(reader) != RES_OK)
				break;
			iAVLRecords.add(std::move(record));
		}

		if (static_cast< size_t >(records_total) != iAVLRecords.size()) {
			LG_WRN(this->logger(), "Could not read some AVL. Read[{}] expected[{}]", iAVLRecords.size(), records_total);
			return RES_NOENT;
		}

		iRecordsSize = records_total;
		iCodec = codec;
		return RES_OK;
	}

	LG_WRN(this->logger(), "Unknown protocol.");
	return RES_NOENT;
}

const size_t Payload::size()
{
	return iRecordsSize;
}

AVLRecords &Payload::records()
{
	return iAVLRecords;
}

Json::Value Payload::toJsonValue()
{
	Json::Value val;
	toJsonImpl(val, true);

	return val;
}

result_t Payload::toJsonImpl(Json::Value &rhs, bool root) const
{
	return iAVLRecords.toJson(rhs, root);
}

result_t Payload::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return iAVLRecords.fromJson(rhs, root);
}

} // namespace tc::parser::teltonika