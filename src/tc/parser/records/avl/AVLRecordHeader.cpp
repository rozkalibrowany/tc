#include <tc/parser/records/avl/AVLRecordHeader.h>
//#include <fmt/format.h>

namespace tc::parser::records::avl {

AVLRecordHeader::AVLRecordHeader(int64_t timestamp, int priority)
  : iTimestamp(timestamp)
  , iPriority(priority)
{
  // nothing to do
}

AVLRecordHeader::AVLRecordHeader()
 : AVLRecordHeader(0LL, 0)
{
  // nothing to do
}

AVLRecordHeader& AVLRecordHeader::operator=(const AVLRecordHeader &rhs)
{
  iTimestamp = rhs.iTimestamp;
  iPriority = rhs.iPriority;
  return *this;
}

void AVLRecordHeader::clear()
{
	iTimestamp = 0LL;
	iPriority = 0;
}

bool AVLRecordHeader::empty() const
{
	return (iTimestamp == 0LL && iPriority == 0);
}

result_t AVLRecordHeader::parse(const std::shared_ptr< Reader > &reader)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iTimestamp = reader->readL(8);
  iPriority = reader->read(1);

  return RES_OK;
}

result_t AVLRecordHeader::parse(const std::shared_ptr< Reader > &reader, int codec)
{
  return RES_NOIMPL;
}

std::string AVLRecordHeader::toString()
{
  return !empty() ? fmt::format("************ Header Record ************\
  	\n\tTimestamp: {}\n\tPriority: {}\n\t", iTimestamp, iPriority) : fmt::format("************ Header Record EMPTY ************\n");
}

result_t AVLRecordHeader::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["Timestamp"] = iTimestamp;
	rhs["Priority"] = iPriority;

	return RES_OK;
}


} // namespace tc::parser::records::avl