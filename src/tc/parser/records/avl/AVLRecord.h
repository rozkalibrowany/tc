#ifndef E4EF0871_8722_4CC4_8A6F_AD01CD2675AF
#define E4EF0871_8722_4CC4_8A6F_AD01CD2675AF

#include <tc/parser/records/avl/AVLRecordHeader.h>
#include <tc/parser/records/io/IoRecord.h>
#include <tc/parser/records/gps/GPSRecord.h>
#include <tc/parser/reader/Reader.h>
#include <tc/parser/common/Result.h>

namespace parser::records::avl {

class AVLRecord;
using AVLRecordSPtr = std::shared_ptr< AVLRecord >;
using AVLRecordList = std::vector< AVLRecordSPtr >;

class AVLRecord {
public:
	AVLRecord();
	AVLRecord(int codec);

	result_t read(const reader::ReaderSPtr &reader);
	result_t set(const int codec);

private:
	int iCodec;
	AVLRecordHeader iRecordHeader;
	gps::GPSRecord iGPSRecord;
	io::IoRecord iRecordIo;
};

class AVLRecords {
public:
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

	virtual void add(const AVLRecordSPtr &rhs);
	virtual void add(const AVLRecord &rhs);

	AVLRecordList &data();
	const AVLRecordList &data() const;

protected:
	AVLRecordList iData;
	AVLRecordSPtr cInvalidEl {std::make_shared< AVLRecord >()};
};


} // namespace parser::records::avl

#endif /* E4EF0871_8722_4CC4_8A6F_AD01CD2675AF */
