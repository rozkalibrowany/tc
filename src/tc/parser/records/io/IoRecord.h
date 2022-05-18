#ifndef B2813421_A4C9_496C_930F_A76CC1C8337F
#define B2813421_A4C9_496C_930F_A76CC1C8337F

#include <fmt/ostream.h>
#include <tc/parser/records/RecordI.h>
#include <tc/server/tcp/LockGuard.h>
#include <json/json.h>

namespace tc::parser::records::io {

class IoRecordProperty;
using IoRecordPropertySPtr = std::shared_ptr< IoRecordProperty >;
using IoRecordsPropertyList = std::vector< IoRecordPropertySPtr >;
using IoRecordsPropertyMap = std::map< int, IoRecordsPropertyList >;

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

	IoRecord(int eventID = 0, int elements = 0);

	virtual ~IoRecord() = default;

	void clear() override;
	bool empty() const override;
	result_t parse(const std::shared_ptr< Reader > &reader, int codec) override;
	result_t parse(const std::shared_ptr< Reader > &reader) override;

	std::string toString() override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	result_t parseFixedSize(const std::shared_ptr< Reader > &reader, int ioIdSize, int byteSize);
	result_t parseVariableSize(const std::shared_ptr< Reader > &reader, int ioIdSize);
	int getIdSize(int codec);

	int iEventID;
	int iElements;
	//SysMutex iMutex;
	ByteSizeList iByteSizes;
	IoRecordsPropertyMap iRecordsMap;
};


} // namespace tc::parser::records::io

#endif /* B2813421_A4C9_496C_930F_A76CC1C8337F */
