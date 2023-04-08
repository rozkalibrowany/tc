#ifndef F25D9B53_659E_4A24_B5BB_B848D1096187
#define F25D9B53_659E_4A24_B5BB_B848D1096187
#include <tc/parser/teltonika/records/io/IoRecord.h>
#include <tc/parser/teltonika/records/io/IoRecordProperty.h>
#include <iterator>

namespace tc::parser::teltonika::records {

IoRecord::IoRecord(int eventID, int elements)
 : iEventID(eventID)
 , iElements(elements)
 , iByteSizes(std::vector< ByteSize >{ByteSize::BYTE_1, ByteSize::BYTE_2, ByteSize::BYTE_4, ByteSize::BYTE_8, ByteSize::BYTE_X})
{
  // nothing to do
}

int IoRecord::getIdSize(int codec)
{
  switch (codec) {
    case 142:
      return 2;
    default:
      return 1;
  }
}

bool IoRecord::empty() const
{
	return iRecordsMap.empty();
}

result_t IoRecord::parse(Reader &reader)
{
  return parse(reader, 8);
}

result_t IoRecord::parse(Reader &reader, int codec)
{
  auto ioIdSize = getIdSize(codec);

  auto eventID = reader.readU(ioIdSize);
  auto elements = reader.readU(ioIdSize);

	result_t res = RES_OK;

	for (auto const b : iByteSizes) {
		if (res != RES_OK) {
			break;
		}

    if (b == ByteSize::BYTE_X && (int) ioIdSize == 2) {
			res |= parseVariableSize(reader, ioIdSize);
		} else {
			res |= parseFixedSize(reader, ioIdSize, b);
		}
	}

	iEventID = eventID;
	iElements = elements;
	return res;
}

result_t IoRecord::parseFixedSize(Reader &reader, int ioIdSize, int byteSize)
{
	//LockGuard g(iMutex);
	auto recordsCount = reader.readU(ioIdSize);
	for (uint i = 0; i < recordsCount; i++) {
		auto id = reader.readU(ioIdSize);
		auto value = reader.read(byteSize);

		iRecordsMap.insert(std::make_pair(byteSize, IoRecordsPropertyList()));
		iRecordsMap.at(byteSize).push_back(std::make_shared<IoRecordProperty>(id, value));
	}
	return RES_OK;
}

result_t IoRecord::parseVariableSize(Reader &reader, int ioIdSize)
{
	auto recordsCount = reader.readU(ioIdSize);

	for (unsigned int i = 0; i < recordsCount; i++) {
		auto id = reader.readU(ioIdSize);
		auto length = reader.readU(2);

		if (reader.offset() >= static_cast<int>(reader.buf()->size()) || (reader.offset() + length) >= static_cast<uint>(reader.buf()->size())) {
			LG_ERR(this->logger(), "Unable to create copy buffer. Offset[{}], length[{}]", reader.offset(), length);
			return RES_NOENT;
		}

		auto beg = reader.buf()->begin() + reader.offset();
		auto end = reader.buf()->begin() + reader.offset() + length;
		iRecordsMap.insert(std::make_pair(BYTE_X, IoRecordsPropertyList()));

		if (id == 10358) {
			auto property = std::make_shared< IoRecordProperty >(id);
			auto buf = std::make_unique<common::Buf>(beg, end);
			Reader readerMcan(std::move(buf));
			if (property->parse(readerMcan) != RES_OK) {
				LG_ERR(this->logger(), "Unable to parse Mcan. Offset[{}], length[{}] buf size {}", readerMcan.offset(), length, readerMcan.buf()->size());
				return RES_NOENT;
			}
			iRecordsMap.at(BYTE_X).push_back(std::make_shared<IoRecordProperty>());
		} else {
			iRecordsMap.at(BYTE_X).push_back(std::make_shared< IoRecordProperty >(id, std::distance(beg, end)));
			reader.skip(std::distance(beg, end));
		}
	}

	return RES_OK;
}

result_t IoRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &el = rhs["IoRecords"] = Json::arrayValue;
	for (auto &rec : iRecordsMap) {
		for (auto &r : rec.second) {
			Json::Value val;
			r->toJson(val, root);
			el.append(val);
		}
		
	}
	return RES_OK;
}

result_t IoRecord::fromJsonImpl(const Json::Value &rhs, bool root)
{
	const auto& io = rhs["IoRecords"];
	if (io.size() == 0) {
		LG_WRN(this->logger(), "IO records object empty");
		return RES_INVARG;
	}

	for (auto const& rec : io) {
		auto property = std::make_shared<IoRecordProperty>();
		if (property->fromJson(rec) != RES_OK)
			continue;
		IoRecordsPropertyList list;
		list.emplace_back(property);
		iRecordsMap.insert(std::make_pair(property->iID, list));
	}

	return RES_OK;
}

} // namespace tc::parser::teltonika::records


#endif /* F25D9B53_659E_4A24_B5BB_B848D1096187 */
