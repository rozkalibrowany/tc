#include <tc/parser/records/avl/AVLRecordHeader.h>
#include <fmt/format.h>

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

result_t AVLRecordHeader::parse(const reader::ReaderSPtr &reader)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iTimestamp = reader->readL(8);
  iPriority = reader->read(1);

  return RES_OK;
}

result_t AVLRecordHeader::parse(const reader::ReaderSPtr &reader, int codec)
{
  return parse(reader);
}

std::string AVLRecordHeader::toString()
{
  return fmt::format("************ Header Record ************\
  \n\tTimestamp: {}\n\tPriority: {}\n\t",
  iTimestamp, iPriority);
}

} // namespace tc::parser::records::avl