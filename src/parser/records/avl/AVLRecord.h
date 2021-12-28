#ifndef E4EF0871_8722_4CC4_8A6F_AD01CD2675AF
#define E4EF0871_8722_4CC4_8A6F_AD01CD2675AF

#include <records/avl/AVLRecordHeader.h>
#include <records/io/IoRecord.h>
#include <records/gps/GPSRecord.h>
#include <reader/Reader.h>
#include <common/Result.h>

namespace parser::records::avl {

class AVLRecord;
using AVLRecordSptr = std::shared_ptr< AVLRecord >;
using AVLRecordList = std::vector< AVLRecordSptr >;

class AVLRecord {
public:
	AVLRecord();
	AVLRecord(int codec);

	result_t read(const reader::ReaderSPtr &reader);
	result_t set(const int codec);

private:
	//template <typename P, typename R>
	//result_t get(R &record);

	int iCodec;
	AVLRecordHeader iRecordHeader;
	gps::GPSRecord iGPSRecord;
	io::IoRecord iRecordIo;
};

/*template <typename P, typename R>
result_t AVLRecord::get(R &rec)
{
	if (iReader == nullptr) {
		return RES_NOENT;
	}

	ParserI<P> *basePtr = new ParserI<P>(iReader);
  auto sptr = std::make_shared< R >(rec);
  return basePtr->parse(sptr);
}*/

class AVLRecords {
public:
	size_t size() const;
	bool empty() const;
	void clear();

	AVLRecordSptr &first();
	AVLRecordSptr &last();

	const AVLRecordSptr &first() const;
	const AVLRecordSptr &last() const;

	AVLRecordList::const_iterator begin() const;
	AVLRecordList::const_iterator end() const;

	AVLRecordList::iterator begin();
	AVLRecordList::iterator end();

	virtual void add(const AVLRecordSptr &rhs);
	virtual void add(const AVLRecord &rhs);

	AVLRecordList &data();
	const AVLRecordList &data() const;

protected:
	AVLRecordList iData;
	AVLRecordSptr cInvalidEl {std::make_shared< AVLRecord >()};
};


} // namespace parser::records::avl

#endif /* E4EF0871_8722_4CC4_8A6F_AD01CD2675AF */
