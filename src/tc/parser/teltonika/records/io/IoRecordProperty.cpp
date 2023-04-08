#include <tc/parser/teltonika/records/io/IoRecordProperty.h>

namespace tc::parser::teltonika::records {

IoRecordProperty::IoRecordProperty(int id, int64_t val)
 : iID(id)
 , iValue(val)
{
	// nothing to do
}

bool IoRecordProperty::empty() const
{
	return iID == 0 && iValue == 0;
}

result_t IoRecordProperty::parse(Reader &reader)
{
	return RES_NOIMPL;
}

result_t IoRecordProperty::parse(Reader &reader, int id_size)
{
	return RES_NOIMPL;
}

result_t IoRecordProperty::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["ID"] = iID;
	rhs["value"] = iValue;

	return RES_OK;
}

result_t IoRecordProperty::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.size() == 0) {
		return RES_INVARG;
	}

	if (rhs.isMember("ID"))
		iID = rhs["ID"].asInt();

	if (rhs.isMember("value"))
		iValue = rhs["value"].asInt64();

	return RES_OK;
}

McanIo::McanIo(int _id, int64_t _val, int _size)
 : id(_id), value(_val), size(_size)
{
	// nothing to do
}

result_t McanIo::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["ID"] = id;
	rhs["value"] = value;
	rhs["size"] = size;

	return RES_OK;
}

result_t McanIo::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.size() == 0) {
		return RES_INVARG;
	}

	if (rhs.isMember("ID"))
		id = rhs["ID"].asInt();

	if (rhs.isMember("value"))
		value = rhs["value"].asInt64();

	return RES_OK;
}

IoMcanProperty::IoMcanProperty(int id)
 : IoRecordProperty(id)
{
	// nothing to do
}

bool IoMcanProperty::empty() const
{
	return iIoElements.empty();
}

result_t IoMcanProperty::parse(Reader &reader)
{
	return RES_NOIMPL;
}

result_t IoMcanProperty::parse(Reader &reader, int id_size)
{
	// auto num_of_elements = reader->read(1);
	auto byte_sizes = std::vector< int >{1, 2, 4, 8};

	for (int b = 0; b < (int) byte_sizes.size(); b++) {
		auto num_of_lists = reader.read(4);
		for (int i = 0; i < num_of_lists; i++) {
			auto id = reader.read(4);
			auto size = byte_sizes[i];
			auto value = reader.read(size);
			iIoElements.emplace_back(McanIo{id, value, size});
		}
	}

	return RES_OK;
}

result_t IoMcanProperty::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &el = rhs["IoMcanProperty"] = Json::arrayValue;
	for (auto &e : iIoElements) {
		Json::Value val;
		e.toJson(val);
		el.append(val);
	}
	return RES_OK;
}

result_t IoMcanProperty::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.getMemberNames().size() == 0) {
		return RES_INVARG;
	}

	for (auto const& prop : rhs.getMemberNames()) {
		McanIo mcan;
		if (mcan.fromJson(prop) != RES_OK)
			continue;
		iIoElements.push_back(std::move(mcan));
	}

	return RES_OK;
}


} // namespace tc::parser::teltonika::records