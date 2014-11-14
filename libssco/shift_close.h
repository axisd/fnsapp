#ifndef _SHIFT_CLOSE
#define _SHIFT_CLOSE

#include "sax_serializer_base.h"
#include "model_shift_close.h"

namespace SSCO
{
	class ShiftClose: public ModelShiftClose, public SaxSerializerBase
	{
	public:
		ShiftClose();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
	};

	typedef boost::shared_ptr<ShiftClose> ShiftClosePtr;
}
#endif