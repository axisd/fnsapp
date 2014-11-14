#ifndef _SHIFT_OPEN
#define _SHIFT_OPEN

#include "sax_serializer_base.h"
#include "model_shift_open.h"

// #include "mysql_connection.h"

namespace SSCO
{
	class ShiftOpen : public ModelShiftOpen, public SaxSerializerBase
	{
	public:
		ShiftOpen();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
	};

	typedef boost::shared_ptr<ShiftOpen> ShiftOpenPtr;
}
#endif
