#ifndef FEE8E53A_FF07_4D39_8BDA_27202A9F053E
#define FEE8E53A_FF07_4D39_8BDA_27202A9F053E

#include <tc/parser/Reader.h>
#include <tc/parser/records/RecordI.h>
#include <list>

namespace tc::parser::records::io {

class IoRecordProperty : public RecordI {
public:
	IoRecordProperty(int id = 0, int64_t val = 0);
	virtual ~IoRecordProperty() = default;

	result_t parse(Reader &reader) override;
	result_t parse(Reader &reader, int id_size) override;

	bool empty() const override;

  int iID;
  int64_t iValue;
	std::string iData;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
};

class McanIo : public parser::JsonI {
public:
	McanIo() = default;
	McanIo(int _id, int64_t _val, int _size);
	McanIo(McanIo&& rhs) = default;

	McanIo &operator=(const McanIo &rhs) = default;
	auto operator<=>(const McanIo &rhs) const = default;

	int id;
	int64_t value;
	int size;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
};

class IoMcanProperty : public IoRecordProperty {
public:

	IoMcanProperty(int id = 0);
	virtual ~IoMcanProperty() = default;

	result_t parse(Reader &reader) override;
	result_t parse(Reader &reader, int id_size) override;

	bool empty() const override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

	std::list< McanIo > iIoElements;
};

} // namespace tc::parser::records::io


#endif /* FEE8E53A_FF07_4D39_8BDA_27202A9F053E */
