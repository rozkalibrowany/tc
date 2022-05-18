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

	result_t parse(const std::shared_ptr< Reader > &reader) override;
	result_t parse(const std::shared_ptr< Reader > &reader, int id_size) override;

	void clear() override final;
	bool empty() const override;
	std::string toString() override;

  int iID;
  int64_t iValue;
	std::string iData;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
};

class McanIo : public parser::JsonI {
public:
	McanIo(int _id, int64_t _val, int _size);
	McanIo &operator=(const McanIo &rhs) = default;
	auto operator<=>(const McanIo &rhs) const = default;

	int id;
	int64_t value;
	int size;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
};

class IoMcanProperty : public IoRecordProperty {
public:

	IoMcanProperty(int id = 0);
	virtual ~IoMcanProperty() = default;

	result_t parse(const std::shared_ptr< Reader > &reader) override;
	result_t parse(const std::shared_ptr< Reader > &reader, int id_size) override;

	bool empty() const override;
	std::string toString() override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	std::list< McanIo > iIoElements;
};

} // namespace tc::parser::records::io


#endif /* FEE8E53A_FF07_4D39_8BDA_27202A9F053E */
