#ifndef F25D9B53_659E_4A24_B5BB_B848D1096187
#define F25D9B53_659E_4A24_B5BB_B848D1096187
#include <tc/parser/records/io/IoRecord.h>
#include <tc/parser/records/io/IoRecordProperty.h>
#include <iterator>

namespace tc::parser::records::io {

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

void IoRecord::clear()
{
	iEventID = 0;
	iElements = 0;
	iRecordsMap.clear();
}

bool IoRecord::empty() const
{
	return iRecordsMap.empty();
}

result_t IoRecord::parse(const std::shared_ptr< Reader > &reader)
{
  return RES_NOIMPL;
}

result_t IoRecord::parse(const std::shared_ptr< Reader > &reader, int codec)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }
  auto ioIdSize = getIdSize(codec);

  auto eventID = reader->readU(ioIdSize);
  auto elements = reader->readU(ioIdSize);

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

result_t IoRecord::parseFixedSize(const std::shared_ptr< Reader > &reader, int ioIdSize, int byteSize)
{
	//LockGuard g(iMutex);
	auto recordsCount = reader->readU(ioIdSize);
	for (uint i = 0; i < recordsCount; i++) {
		auto id = reader->readU(ioIdSize);
		auto value = reader->read(byteSize);
		iRecordsMap.insert(std::make_pair(byteSize, IoRecordsPropertyList()));
		iRecordsMap.at(byteSize).push_back(std::make_shared<IoRecordProperty>(id, value));
	}
	return RES_OK;
}

result_t IoRecord::parseVariableSize(const std::shared_ptr< Reader > &reader, int ioIdSize)
{
	//LockGuard g(iMutex);
	auto recordsCount = reader->readU(ioIdSize);

	for (unsigned int i = 0; i < recordsCount; i++) {
		auto id = reader->readU(ioIdSize);
		auto length = reader->readU(2);

		if (reader->offset() >= static_cast<int>(reader->buf()->size()) || (reader->offset() + length) >= static_cast<uint>(reader->buf()->size())) {
			LG_ERR(this->logger(), "Unable to create copy buffer. Offset[{}], length[{}]", reader->offset(), length);
			return RES_NOENT;
		}

		///LG_NFO(this->logger(), "Io Record parseVariableSize iOffset {} length {} buf size {}", reader->offset(), length, reader->buf()->size());

		auto beg = reader->buf()->begin() + reader->offset();
		auto end = reader->buf()->begin() + reader->offset() + length;
		iRecordsMap.insert(std::make_pair(BYTE_X, IoRecordsPropertyList()));

		if (id == 10358) {
			auto property = std::make_shared< IoRecordProperty >(id);
			auto buf = std::make_unique<Buf>(beg, end);
			auto readerMcan = std::make_shared<Reader>(std::move(buf));
			if (property->parse(readerMcan) != RES_OK) {
				LG_ERR(this->logger(), "Unable to parse Mcan. Offset[{}], length[{}] buf size {}", readerMcan->offset(), length, readerMcan->buf()->size());
				return RES_NOENT;
			}
			iRecordsMap.at(BYTE_X).push_back(std::make_shared<IoRecordProperty>());
		} else {
			iRecordsMap.at(BYTE_X).push_back(std::make_shared< IoRecordProperty >(id, std::distance(beg, end)));
			reader->skip(std::distance(beg, end));
		}
	}

	return RES_OK;
}

std::string IoRecord::toString()
{
	if (empty()) {
		return std::string();
	}

	auto s = fmt::format("*************** Io Record ***************\
  \n\tEvent ID: {}\n", iEventID);

  if (iRecordsMap[ByteSize::BYTE_1].size() > 0) {
    s += std::string("\tByte 1");
    for (auto r : iRecordsMap[ByteSize::BYTE_1]) {
      s += fmt::format("\n\t\tID: {}, val: {}", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_2].size() > 0) {
    s += std::string("\n\tByte 2");
    for (auto r : iRecordsMap[ByteSize::BYTE_2]) {
      s += fmt::format("\n\t\tID: {}, val: {}", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_4].size() > 0) {
    s += std::string("\n\tByte 4");
    for (auto r : iRecordsMap[ByteSize::BYTE_4]) {
      s += fmt::format("\n\t\tID: {}, val: {}", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_8].size() > 0) {
    s += std::string("\n\tByte 8");
    for (auto r : iRecordsMap[ByteSize::BYTE_8]) {
      s += fmt::format("\n\t\tID: {}, val: {}", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_X].size() > 0) {
    s += std::string("\n\tByte X");
    for (auto r : iRecordsMap[ByteSize::BYTE_X]) {
      s += fmt::format("\n\t\tID: {}, val: {}", r->iID, r->iValue);
    }
  }

  return s;
}

result_t IoRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &el = rhs["IoRecords"] = Json::arrayValue;
	for (auto &r : iRecordsMap) {
		Json::Value val;
		r.second.front()->toJson(val);
		el.append(val);
	}
	return RES_OK;
}

} // namespace tc::parser::records::io


#endif /* F25D9B53_659E_4A24_B5BB_B848D1096187 */
