#ifndef E4EF0871_8722_4CC4_8A6F_AD01CD2675AF
#define E4EF0871_8722_4CC4_8A6F_AD01CD2675AF

#include <tc/parser/records/avl/AVLRecordHeader.h>
#include <tc/parser/records/io/IoRecord.h>
#include <tc/parser/records/gps/GPSRecord.h>
#include <tc/parser/Reader.h>

namespace tc::parser::records::avl {

class AVLRecord;
using AVLRecordSPtr = std::shared_ptr< AVLRecord >;
using AVLRecordList = std::vector< AVLRecordSPtr >;

class AVLRecord : public tc::LogI {
public:
	AVLRecord();
	AVLRecord(int codec);

	AVLRecord &operator=(AVLRecord &&rhs);

	virtual ~AVLRecord() = default;

	result_t read(const std::shared_ptr< Reader > &reader);
	result_t set(const int codec);

	int iCodec;
	AVLRecordHeader iRecordHeader;
	gps::GPSRecord iGPSRecord;
	io::IoRecord iRecordIo;
};
class AVLRecords {
public:
	~AVLRecords() = default;

	size_t size() const;
	bool empty() const;
	void clear();

	AVLRecordSPtr &first();
	AVLRecordSPtr &last();

	const AVLRecordSPtr &first() const;
	const AVLRecordSPtr &last() const;

	AVLRecordList::const_iterator begin() const;
	AVLRecordList::const_iterator end() const;

	AVLRecordList::iterator begin();
	AVLRecordList::iterator end();

	void add(const AVLRecordSPtr &rhs);
	void add(const AVLRecord &rhs);

	AVLRecordList &data();
	const AVLRecordList &data() const;

protected:
	AVLRecordList iData;
	AVLRecordSPtr cInvalidEl {std::make_shared< AVLRecord >()};
};


} // namespace tc::parser::records::avl

#endif /* E4EF0871_8722_4CC4_8A6F_AD01CD2675AF */
