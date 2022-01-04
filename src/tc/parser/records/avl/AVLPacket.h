#ifndef B6ED04E5_AB7F_4453_BE91_514FC5836D3B
#define B6ED04E5_AB7F_4453_BE91_514FC5836D3B

#include <records/avl/AVLRecord.h>

namespace parser::records::avl {

class AVLPacket;
using PacketUPtr = std::unique_ptr< AVLPacket >;

class AVLPacket {
public:

	AVLPacket(int codec, int recordC, AVLRecords &records)
	 : iCodec(codec)
	 , iRecordC(recordC)
	 , iRecords(records)
	{}

	int iCodec;
	int iRecordC;
	AVLRecords iRecords;
};


} // namespace parser::records::avl

#endif /* B6ED04E5_AB7F_4453_BE91_514FC5836D3B */
