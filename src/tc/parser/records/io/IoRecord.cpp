#include <tc/parser/records/io/IoRecord.h>
#include <tc/parser/records/io/IoRecordProperty.h>
#include <fmt/format.h>

namespace tc::parser::records::io {

IoRecord::IoRecord(int eventID, int elements)
 : iEventID(eventID)
 , iElements(elements)
 , iByteSizes(std::vector< ByteSize >{ByteSize::BYTE_1, ByteSize::BYTE_2, ByteSize::BYTE_4, ByteSize::BYTE_8, ByteSize::BYTE_X})
{
  // nothing to do
}

IoRecord::IoRecord()
 : IoRecord(0, 0)
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

result_t IoRecord::parse(const reader::ReaderSPtr &reader)
{
  return RES_NOIMPL;
}

result_t IoRecord::parse(const reader::ReaderSPtr &reader, int codec)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }
  auto ioIdSize = getIdSize(codec);

  iEventID = static_cast<int>(reader->readU(ioIdSize));
  iElements = static_cast<int>(reader->readU(ioIdSize));

  IoRecordsPropertyMap recordsMap;
	result_t res = RES_OK;

	for (auto b : iByteSizes) {
		if (res != RES_OK) {
			break;
		}

		IoRecordsPropertyList propertyList;
    if (b == ByteSize::BYTE_X && (int) ioIdSize == 2) {
			res |= parseVariableSize(reader, propertyList, ioIdSize);
		} else {
			res |= parseFixedSize(reader, propertyList, ioIdSize, b);
		}
		recordsMap.insert(std::make_pair(b, propertyList));
	}

	iRecordsMap = std::move(recordsMap);
  return res;
}

result_t IoRecord::parseFixedSize(const reader::ReaderSPtr &reader, IoRecordsPropertyList &list, int ioIdSize, int byteSize)
{
	int recordsCount = static_cast< int>(reader->readU(ioIdSize));

	for (int i = 0; i < recordsCount; i++) {
		auto id = reader->readU(ioIdSize);
		auto value = reader->read(byteSize);
		auto ioProperty = std::make_shared< IoRecordProperty >(id, value);
		list.push_back(std::move(ioProperty));
	}
	return RES_OK;
}

result_t IoRecord::parseVariableSize(const reader::ReaderSPtr &reader, IoRecordsPropertyList &list, int ioIdSize)
{
	int recordsCount = static_cast< int>(reader->readU(ioIdSize));
	std::shared_ptr< IoRecordProperty > ioProperty;

	for (int i = 0; i < recordsCount; i++) {
		auto id = reader->readU(ioIdSize);
		auto length = reader->readU(2);

		auto begin = reader->iBuf.begin() + reader->iOffset;
		auto end = reader->iBuf.begin() + reader->iOffset + length;

		if (begin == reader->iBuf.end() || end == reader->iBuf.end()) {
			SPDLOG_LOGGER_ERROR(this->logger(), "Unable to create copy buffer. Offset[{}], length[{}]", reader->iOffset, length);
			return RES_NOENT;
		}

		reader::Buf subBuf(begin, end);

		if (id == 10358) {
			ioProperty = std::make_shared< IoMcanProperty >(id);
			auto readerMcan = std::make_shared<reader::Reader>(subBuf);
			if (ioProperty->parse(readerMcan) != RES_OK) {
				SPDLOG_LOGGER_ERROR(this->logger(), "Unable to parse Mcan. Offset[{}], length[{}]", reader->iOffset, length);
				return RES_NOENT;
			}
		} else {
			ioProperty = std::make_shared< IoRecordProperty >(id, subBuf.size());
			reader->skip(subBuf.size());
		}

		list.push_back(ioProperty);
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

} // namespace tc::parser::records::io