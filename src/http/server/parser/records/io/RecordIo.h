#ifndef B2813421_A4C9_496C_930F_A76CC1C8337F
#define B2813421_A4C9_496C_930F_A76CC1C8337F

#include <records/io/RecordIoLists.h>

namespace parser::records::io {

class RecordIo {
public:
	RecordIo(int eventID, int elements, RecordIoLists &recordsList) : iEventID(eventID), iElements(elements), iRecordsLists(recordsList) {}

	int iEventID;
	int iElements;
	RecordIoLists iRecordsLists;
};


} // namespace parser::records::io


#endif /* B2813421_A4C9_496C_930F_A76CC1C8337F */
