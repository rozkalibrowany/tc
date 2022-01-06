#include <tc/parser/Packet.h>
#include <tc/parser/records/avl/AVLRecord.h>
#include <spdlog/spdlog.h>
#include <algorithm>

namespace tc::parser {

int Packet::PACKET_INIT_MAX_SIZE = 50;

result_t Packet::parse(Buf &buf)
{
	if (contains(buf, '8') || contains(buf, '\x8E')) {
		int offset = -1;
		iAVLRecordsUPtr.reset(new AVLRecords);
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
			iAVLRecordsUPtr->add(record);
		}

		if (records_total != (int) iAVLRecordsUPtr->size()) {
			return RES_NOENT;
		}

		iCodec = codec;
		iRecordsTotal = records_total;
		spdlog::info("AVL Packet, total records: {} codec: {}", iRecordsTotal, iCodec);
		iReader = std::move(reader);
		return RES_OK;
	}

	spdlog::warn("Unknow protocol.");
	return RES_NOENT;
}

result_t Packet::parse(unsigned char* cbuf, size_t size)
{
	Buf buffer(cbuf, size);

	if ((int) size < Packet::PACKET_INIT_MAX_SIZE) {
		spdlog::info("Got message containing IMEI number.");
		return parseImei(buffer);
	}

	spdlog::info("Got message containing AVL packet.");
  return parse(buffer);
}

result_t Packet::parseImei(Buf &buf)
{
	if (buf.size() <= 2) {
		return RES_INVARG;
	}

	auto imei_length = ((buf[0]) << 8) | ((buf[1]) << 0);

	buf.iBuf.erase(buf.begin(), buf.begin() + 2);
	iImei.iLength = imei_length;
	iImei.iData = &buf.iBuf.front();

	return RES_OK;
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

const ReaderSPtr &Packet::creader() const
{
	return iReader;
}

AVLRecordsUPtr Packet::records()
{
	return std::move(iAVLRecordsUPtr);
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

} // namespace tc::parser