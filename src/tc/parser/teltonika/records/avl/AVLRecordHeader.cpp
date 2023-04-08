#include <tc/parser/teltonika/records/avl/AVLRecordHeader.h>
//#include <fmt/format.h>

namespace tc::parser::teltonika::records {

AVLRecordHeader::AVLRecordHeader(int64_t timestamp, int priority)
  : iTimestamp(timestamp)
  , iPriority(priority)
{
  // nothing to do
}

AVLRecordHeader& AVLRecordHeader::operator=(const AVLRecordHeader &rhs)
{
  iTimestamp = rhs.iTimestamp;
  iPriority = rhs.iPriority;
  return *this;
}

bool AVLRecordHeader::empty() const
{
	return (!iTimestamp.has_value() && !iPriority.has_value());
}

result_t AVLRecordHeader::parse(Reader &reader)
{
	auto timestamp = reader.readL(8);
	if (timestamp != 0)
		iTimestamp = timestamp;

	auto priority = reader.read(1);
	iPriority = priority;

	return RES_OK;
}

result_t AVLRecordHeader::parse(Reader &reader, int codec)
{
  return RES_NOIMPL;
}

result_t AVLRecordHeader::toJsonImpl(Json::Value &rhs, bool root) const
{
	Json::Value header;
	if (iTimestamp.has_value())
		header["Timestamp"] = iTimestamp.value();
	
	if (iPriority.has_value())
		header["Priority"] = iPriority.value();

	rhs["Header"] = header;

	return RES_OK;
}

result_t AVLRecordHeader::fromJsonImpl(const Json::Value &rhs, bool root)
{
	const auto& header = rhs["Header"];
	if (header.size() == 0) {
		LG_WRN(this->logger(), "Header record object empty");
		return RES_INVARG;
	}
	
	iTimestamp = header["Timestamp"].asInt64();
	iPriority = header["Priority"].asInt();

	return RES_OK;
}

} // namespace tc::parser::teltonika::records