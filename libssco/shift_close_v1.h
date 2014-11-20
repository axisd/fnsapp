#ifndef _SHIFT_CLOSE_V1
#define _SHIFT_CLOSE_V1

#include "sax_serializer_base.h"
#include "model_shift_close_v1.h"

namespace SSCO
{
    class ShiftCloseV1: public ModelShiftCloseV1, public SaxSerializerBase
	{
	public:
        ShiftCloseV1();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
	};

    typedef boost::shared_ptr<ShiftCloseV1> ShiftCloseV1Ptr;
}
#endif
