#ifndef A87F3B87_85E1_401D_A8DE_7E4A1BF8CB6D
#define A87F3B87_85E1_401D_A8DE_7E4A1BF8CB6D

#include <records/io/RecordIo.h>
#include <records/io/RecordIoLists.h>
#include <records/io/RecordIoProperty.h>
#include <records/ParserI.h>
#include <reader/Reader.h>
#include <type_traits>

namespace parser::records::io {

template<typename T>
class IoParser : public ParserI< T > {
public:

  IoParser(reader::ReaderSPtr &reader, int idsize = 2) : ParserI<T>(reader), iIDSize(idsize) {}
  virtual ~IoParser() {}

protected:
  virtual result_t parse(std::shared_ptr<T> &record);
  virtual result_t parse(int fixedSize);

private:
  int iIDSize;
};

template<typename T>
result_t IoParser<T>::parse(std::shared_ptr<T> &record)
{
  if constexpr (!std::is_same_v<T, RecordIo> || record == nullptr) {
    return RES_NOENT;
  }

  auto ioRecords = std::make_shared< RecordIoLists >();
  auto ioRecordsProperty = RecordsIoProperty();
  result_t res = RES_OK;
  res |= parse(ioRecords->iByte1List, 1);
  res |= parse(ioRecords->iByte2List, 2);
  res |= parse(ioRecords->iByte4List, 4);
  res |= parse(ioRecords->iByte8List, 8);

  if(iIDSize == 2){
    //record.setByteList_XList(ParseVariableSizeData());
  }

  return res;
}

result_t IoParser<T>::parse(RecordsIoProperty &records, const int size)
{
  auto recCount = iReader->readU(iIDSize);
  int val, id;

  for (int i = 0; i < recCount; i++) {
    id = iReader->readU(iIDSize);
    val = iReader->read(size);
    auto record = std::make_shared< RecordIoProperty >(id, val);
    records.push_back(record);
  }
  return RES_OK;
}

} // namespace parser::records::io

#endif /* A87F3B87_85E1_401D_A8DE_7E4A1BF8CB6D */
