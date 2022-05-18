#include <tc/parser/records/io/IoRecordProperty.h>

namespace tc::parser::records::io {

IoRecordProperty::IoRecordProperty(int id, int64_t val)
 : iID(id)
 , iValue(val)
{
	// nothing to do
}

void IoRecordProperty::clear()
{
	iID = 0;
	iValue = 0;
	iData.clear();
}

bool IoRecordProperty::empty() const
{
	return iID == 0 && iValue == 0;
}

result_t IoRecordProperty::parse(const std::shared_ptr< Reader > &reader)
{
	return RES_NOIMPL;
}

result_t IoRecordProperty::parse(const std::shared_ptr< Reader > &reader, int id_size)
{
	return RES_NOIMPL;
}

std::string IoRecordProperty::toString()
{
	if (empty()) {
		return std::string();
	}

	auto s = fmt::format("************ Io Record Property ************\n");
	s += fmt::format("\tID: {}\n\tValue: {}\n", iID, iValue);

	if (iData.empty() == false) {
		s += fmt::format("\tData: {}", iData);
	}

	return s;
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

result_t IoMcanProperty::parse(const std::shared_ptr< Reader > &reader)
{
	return RES_NOIMPL;
}

result_t IoMcanProperty::parse(const std::shared_ptr< Reader > &reader, int id_size)
{
	if (reader == nullptr) {
		return RES_INVARG;
	}

	// auto num_of_elements = reader->read(1);
	auto byte_sizes = std::vector< int >{1, 2, 4, 8};

	for (int b = 0; b < (int) byte_sizes.size(); b++) {
		auto num_of_lists = reader->read(4);
		for (int i = 0; i < num_of_lists; i++) {
			auto id = reader->read(4);
			auto size = byte_sizes[i];
			auto value = reader->read(size);
			iIoElements.emplace_back(McanIo{id, size, value});
		}
	}

	return RES_OK;
}

std::string IoMcanProperty::toString()
{
	if (empty()) {
		return std::string();
	}

	auto s = fmt::format("************ Io Mcan Property ************\n");

	for (auto &e : iIoElements) {
		s += fmt::format("\tElement size: {} \n\t\tid: {}, value: {}", e.size, e.id, e.value);
	}

	return s;
}

} // namespace tc::parser::records::io