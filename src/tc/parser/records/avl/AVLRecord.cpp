#include <tc/parser/records/avl/AVLRecord.h>

namespace tc::parser::records::avl {

AVLRecord::AVLRecord(int codec)
 : tc::LogI("console")
 , iCodec(codec)
{
  // nothing to do
}

result_t AVLRecord::read(const std::shared_ptr< Reader > &reader)
{
	if (reader == nullptr) {
    return RES_NOENT;
  }
  result_t res = RES_OK;
  if((res = iRecordHeader.parse(reader)) != RES_OK) {
		return res;
	}
	if((res = iGPSRecord.parse(reader)) != RES_OK) {
		return res;
	}
	if((res = iRecordIo.parse(reader, iCodec)) != RES_OK) {
		return res;
	}

	//LG_NFO(this->logger(), "\n{}{}{}", iRecordHeader.toString(), iGPSRecord.toString(), iRecordIo.toString());
  return res;
}

result_t AVLRecord::set(const int codec)
{
  iCodec = codec;
  return RES_OK;
}

result_t AVLRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &arr = rhs["AVLRecord"] = Json::arrayValue;

  Json::Value val;
	iRecordHeader.toJson(val);
	arr.append(val);
  iGPSRecord.toJson(val);
  arr.append(val);
  iRecordIo.toJson(val);
  arr.append(val);

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

AVLRecordSPtr &AVLRecords::first()
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

AVLRecordSPtr &AVLRecords::last()
{
  return size() > 0 ? iData.back() : cInvalidEl;
}


const AVLRecordSPtr &AVLRecords::first() const
{
  return size() > 0 ? iData.front() : cInvalidEl;
}

const AVLRecordSPtr &AVLRecords::last() const
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

void AVLRecords::add(const AVLRecordSPtr &rhs)
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


} // namespace tc::parser::AVLRecords::avl