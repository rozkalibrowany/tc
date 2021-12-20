#include <records/avl/AVLRecord.h>

namespace parser::records::avl {

Record::Record(reader::ReaderSPtr &reader, int codec)
 : iReader(reader)
 , iCodec(codec)
{
  // nothing to do
}

Record::Record()
 : iReader(nullptr)
 , iCodec(0)
{}


size_t Records::size() const
{
  return iData.size();
}

bool Records::empty() const
{
  return iData.empty();
}

void Records::clear()
{
  iData.clear();
}

Records::RecordSptr &Records::first()
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

Records::RecordSptr &Records::last()
{
  return size() > 0 ? iData.back() : cInvalidEl;
}


const Records::RecordSptr &Records::first() const
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

const Records::RecordSptr &Records::last() const
{
  return size() > 0 ? iData.back() : cInvalidEl;
}

Records::RecordList::const_iterator Records::begin() const
{
  return iData.begin();
}

Records::RecordList::const_iterator Records::end() const
{
  return iData.end();
}

Records::RecordList::iterator Records::begin()
{
  return iData.begin();
}

Records::RecordList::iterator Records::end()
{
  return iData.end();
}

void Records::add(const RecordSptr &rhs)
{
  iData.push_back(rhs);
}

void Records::add(const Record &rhs)
{
  add(std::make_shared< Record >(rhs));
}

Records::RecordList &Records::data()
{
  return iData;
}

const Records::RecordList &Records::data() const
{
  return iData;
}


} // namespace parser::records::avl