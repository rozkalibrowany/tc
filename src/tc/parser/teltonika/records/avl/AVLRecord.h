#ifndef E4EF0871_8722_4CC4_8A6F_AD01CD2675AF
#define E4EF0871_8722_4CC4_8A6F_AD01CD2675AF

#include <tc/parser/teltonika/records/avl/AVLRecordHeader.h>
#include <tc/parser/teltonika/records/io/IoRecord.h>
#include <tc/parser/teltonika/records/gps/GPSRecord.h>
#include <tc/parser/Reader.h>

namespace tc::parser::teltonika::records {

class AVLRecord : public tc::LogI, public parser::JsonI {
public:
	AVLRecord(int codec = 142);
	AVLRecord(AVLRecord&& rhs);

	result_t read(Reader &reader);
	result_t set(const int codec);

	Json::Value toJsonValue();

	AVLRecordHeader iRecordHeader;
	GPSRecord iGPSRecord;
	IoRecord iRecordIo;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	
private:
	int iCodec;
};

class AVLRecords : public tc::LogI, public parser::JsonI {
public:
	size_t size() const;
	bool empty() const;
	void clear();

	AVLRecord &first();
	AVLRecord &last();

	const AVLRecord &first() const;
	const AVLRecord &last() const;

	std::vector< AVLRecord >::const_iterator begin() const;
	std::vector< AVLRecord >::const_iterator end() const;

	std::vector< AVLRecord >::iterator begin();
	std::vector< AVLRecord >::iterator end();

	void add(AVLRecord &&rhs);

	std::vector< AVLRecord > &data();
	const std::vector< AVLRecord > &cdata() const;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

	std::vector< AVLRecord > iRecords;
};

} // namespace tc::parser::teltonika::records

#endif /* E4EF0871_8722_4CC4_8A6F_AD01CD2675AF */
