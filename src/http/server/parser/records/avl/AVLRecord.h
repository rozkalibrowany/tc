#ifndef E4EF0871_8722_4CC4_8A6F_AD01CD2675AF
#define E4EF0871_8722_4CC4_8A6F_AD01CD2675AF

#include <records/avl/AVLRecordHeader.h>
#include <records/io/RecordIo.h>
#include <records/gps/GPSRecord.h>
#include <records/gps/GPSParser.h>
#include <records/ParserI.h>
#include <reader/Reader.h>
#include <common/Result.h>

namespace parser::records::avl {

class Record  {
public:
	Record();
	Record(reader::ReaderSPtr &reader, int codec);

	result_t get(RecordHeader &val) {
		return RES_OK;
	}
	result_t get(gps::GPSRecord &val) {
		ParserI<gps::GPSParser> *basePtr = new ParserI<gps::GPSParser>(iReader);
		auto sptr = std::make_shared< gps::GPSRecord >(val);
		return basePtr->parse(sptr);
	}
	result_t get(io::RecordIo &val) { return RES_OK; };


	//RecordHeader iRecordHeader;
	//gps::GPSRecord iGPSRecord;
	//io::RecordIo iRecordIo;

	reader::ReaderSPtr iReader;
	int iCodec;
};

class Records {
public:
	using RecordSptr = std::shared_ptr< Record >;
	using RecordList = std::vector< RecordSptr >;

	size_t size() const;
	bool empty() const;
	void clear();

	RecordSptr &first();
	RecordSptr &last();

	const RecordSptr &first() const;
	const RecordSptr &last() const;

	Records::RecordList::const_iterator begin() const;
	Records::RecordList::const_iterator end() const;

	Records::RecordList::iterator begin();
	Records::RecordList::iterator end();

	virtual void add(const RecordSptr &rhs);
	virtual void add(const Record &rhs);

	RecordList &data();
	const RecordList &data() const;

protected:
	RecordList iData;
	RecordSptr cInvalidEl {std::make_shared< Record >()};


};


} // namespace parser::records::avl

#endif /* E4EF0871_8722_4CC4_8A6F_AD01CD2675AF */
