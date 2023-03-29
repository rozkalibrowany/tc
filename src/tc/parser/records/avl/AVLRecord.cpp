#include <tc/parser/records/avl/AVLRecord.h>

namespace tc::parser::records::avl {

AVLRecord::AVLRecord(int codec)
 : tc::LogI("console")
 , iCodec(codec)
{
  // nothing to do
}

AVLRecord::AVLRecord(AVLRecord&& rhs)
 : tc::LogI("console")
 , iRecordHeader(std::move(rhs.iRecordHeader))
 , iGPSRecord(std::move(rhs.iGPSRecord))
 , iRecordIo(std::move(rhs.iRecordIo))
{
  // nothing to do
}

result_t AVLRecord::read(Reader &reader)
{
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

  return res;
}

result_t AVLRecord::set(const int codec)
{
  iCodec = codec;
  return RES_OK;
}

Json::Value AVLRecord::toJsonValue()
{
	Json::Value val;
	toJsonImpl(val, true);

	return val;
}

result_t AVLRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &arr = rhs["Record"] = Json::arrayValue;

	if (!iRecordHeader.empty()) {
 		Json::Value val;
		iRecordHeader.toJson(val, root);
		arr.append(val);
	}

	if (!iGPSRecord.empty()) {
		Json::Value val;
		iGPSRecord.toJson(val, root);
		arr.append(val);
	}
  
	if (!iRecordIo.empty()) {
		Json::Value val;
		iRecordIo.toJson(val, root);
		arr.append(val);
	}

	return RES_OK;
}

result_t AVLRecord::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.size() == 0) {
		return RES_INVARG;
	}

	bool res = false;
	if (rhs.isMember("Header")) {
		res |= iRecordHeader.fromJson(rhs) == RES_OK ? true : false;
	}

	if (rhs.isMember("GPS")) {
		res |= iGPSRecord.fromJson(rhs) == RES_OK ? true : false;
	}
		
	if (rhs.isMember("IoRecords")) {
		res |= iRecordIo.fromJson(rhs) == RES_OK ? true : false;
	}

	return res ? RES_OK : RES_NOENT;
}

size_t AVLRecords::size() const
{
  return iRecords.size();
}

bool AVLRecords::empty() const
{
  return iRecords.empty();
}

void AVLRecords::clear()
{
  iRecords.clear();
}

AVLRecord &AVLRecords::first()
{
  return iRecords.front();
}

AVLRecord &AVLRecords::last()
{
	return iRecords.back();
}

const AVLRecord &AVLRecords::first() const
{
	return iRecords.front();
}

const AVLRecord &AVLRecords::last() const
{
  return iRecords.back();
}

std::vector< AVLRecord >::const_iterator AVLRecords::begin() const
{
  return iRecords.begin();
}

std::vector< AVLRecord >::const_iterator AVLRecords::end() const
{
  return iRecords.end();
}

std::vector< AVLRecord >::iterator AVLRecords::begin()
{
  return iRecords.begin();
}

std::vector< AVLRecord >::iterator AVLRecords::end()
{
  return iRecords.end();
}

void AVLRecords::add(AVLRecord &&rhs)
{
  iRecords.emplace_back(std::move(rhs));
}

std::vector< AVLRecord > &AVLRecords::data()
{
  return iRecords;
}

const std::vector< AVLRecord > &AVLRecords::cdata() const
{
  return iRecords;
}

result_t AVLRecords::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &arr = rhs["Records"] = Json::arrayValue;
	Json::Value val;
  for (const auto& rec : iRecords) {
		if (rec.toJson(val, root) != RES_OK)
			continue;
		arr.append(val);
	}

	return RES_OK;
}

result_t AVLRecords::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.size() == 0) {
		return RES_INVARG;
	}
	for (auto const& rec : rhs) {
		AVLRecord record;
		if (record.fromJson(rec) != RES_OK) {
			LG_ERR(this->logger(), "Error reading AVL records from json.");
			continue;
		}

		add(std::move(record));
	}

	return RES_OK;
}

} // namespace tc::parser::AVLRecords::avl