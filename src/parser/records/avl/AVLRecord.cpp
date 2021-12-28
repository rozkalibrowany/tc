#include <records/avl/AVLRecord.h>

namespace parser::records::avl {

AVLRecord::AVLRecord(int codec)
 : iCodec(codec)
{
  // nothing to do
}

AVLRecord::AVLRecord()
 : AVLRecord(0)
{
  // nothing to do
}

result_t AVLRecord::read(const reader::ReaderSPtr &reader)
{
  if (reader == nullptr) {
    return RES_NOENT;
  }
  result_t res = RES_OK;
  res |= iRecordHeader.parse(reader);
  std::cout << "iRecordHeader reader offset: " << reader->iOffset << std::endl;
  res |= iGPSRecord.parse(reader);
  std::cout << "iGPSRecord reader offset: " << reader->iOffset << std::endl;
  res |= iRecordIo.parse(reader, iCodec);
  std::cout << "iRecordIo reader offset: " << reader->iOffset << std::endl;

  return res;
}

result_t AVLRecord::set(const int codec)
{
  iCodec = codec;
  return RES_OK;
}

size_t AVLRecords::size() const
{
  return iData.size();
}

bool AVLRecords::empty() const
{
  return iData.empty();
}

void AVLRecords::clear()
{
  iData.clear();
}

AVLRecordSptr &AVLRecords::first()
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

AVLRecordSptr &AVLRecords::last()
{
  return size() > 0 ? iData.back() : cInvalidEl;
}


const AVLRecordSptr &AVLRecords::first() const
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

const AVLRecordSptr &AVLRecords::last() const
{
  return size() > 0 ? iData.back() : cInvalidEl;
}

AVLRecordList::const_iterator AVLRecords::begin() const
{
  return iData.begin();
}

AVLRecordList::const_iterator AVLRecords::end() const
{
  return iData.end();
}

AVLRecordList::iterator AVLRecords::begin()
{
  return iData.begin();
}

AVLRecordList::iterator AVLRecords::end()
{
  return iData.end();
}

void AVLRecords::add(const AVLRecordSptr &rhs)
{
  iData.push_back(rhs);
}

void AVLRecords::add(const AVLRecord &rhs)
{
  add(std::make_shared< AVLRecord >(rhs));
}

AVLRecordList &AVLRecords::data()
{
  return iData;
}

const AVLRecordList &AVLRecords::data() const
{
  return iData;
}


} // namespace parser::AVLRecords::avl