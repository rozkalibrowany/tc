#ifndef B2813421_A4C9_496C_930F_A76CC1C8337F
#define B2813421_A4C9_496C_930F_A76CC1C8337F

#include <tc/parser/records/RecordI.h>
#include <list>
#include <map>

namespace tc::parser::records::io {

class IoRecordProperty;
using IoRecordPropertySPtr = std::shared_ptr< IoRecordProperty >;
using IoRecordsPropertyList = std::vector< IoRecordPropertySPtr >;
using IoRecordsMap = std::map< int, IoRecordsPropertyList >;

class IoRecord : public RecordI {
public:
	enum ByteSize {
		BYTE_1 = 1,
		BYTE_2 = 2,
		BYTE_4 = 4,
		BYTE_8 = 8,
		BYTE_X
	};

	using ByteSizeList = std::vector < ByteSize >;

	IoRecord(int eventID, int elements);
	IoRecord();

	virtual ~IoRecord() = default;

	bool empty() const override;
	result_t parse(const reader::ReaderSPtr &reader, int codec) override;
	result_t parse(const reader::ReaderSPtr &reader) override;

	std::string toString() override;

private:
	int getIdSize(int codec);

	int iEventID;
	int iElements;
	ByteSizeList iByteSizes;
	IoRecordsMap iRecordsMap;
};

class IoRecordProperty {
public:
  IoRecordProperty(int id, int64_t val) : iID(id), iValue(val) {}
  IoRecordProperty() : IoRecordProperty(0, 0LL) {}

	result_t parse(const reader::ReaderSPtr &reader, int id_size);
	result_t parse(const reader::ReaderSPtr &reader, int id_size, int val_size);

  int iID;
  int64_t iValue;
};

} // namespace tc::parser::records::io

#endif /* B2813421_A4C9_496C_930F_A76CC1C8337F */
