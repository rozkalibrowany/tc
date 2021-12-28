#ifndef B2813421_A4C9_496C_930F_A76CC1C8337F
#define B2813421_A4C9_496C_930F_A76CC1C8337F

#include <records/RecordI.h>
#include <list>
#include <map>

namespace parser::records::io {

class RecordIoProperty;
using RecordIoPropertySptr = std::shared_ptr< RecordIoProperty >;
using RecordsIoPropertyList = std::vector< RecordIoPropertySptr >;
using RecordsIoMap = std::map< int, RecordsIoPropertyList >;



class IoRecord : public RecordI< IoRecord > {
public:
	enum ByteSize {
		BYTE_1 = 1,
		BYTE_2 = 2,
		BYTE_4 = 4,
		BYTE_8 = 8,
		BYTE_X
	};

	IoRecord(int eventID, int elements);
	IoRecord();

	virtual ~IoRecord() = default;

	virtual result_t parse(const reader::ReaderSPtr &reader, int codec);
	virtual result_t parse(const reader::ReaderSPtr &reader) { return RES_OK; };

private:
	int iEventID;
	int iElements;
	std::vector< ByteSize > iByteSizes;
	RecordsIoMap iRecordsMap;
};

class RecordIoProperty {
public:


  RecordIoProperty(int id, int64_t val) : iID(id), iValue(val) {}
  RecordIoProperty() : RecordIoProperty(0, 0LL) {}

	result_t parse(const reader::ReaderSPtr &reader, int id_size);
	result_t parse(const reader::ReaderSPtr &reader, int id_size, int val_size);

  int iID;
  int64_t iValue;
};


} // namespace parser::records::io


#endif /* B2813421_A4C9_496C_930F_A76CC1C8337F */
