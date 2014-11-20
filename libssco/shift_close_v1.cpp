#include "shift_close_v1.h"

using namespace SSCO;

ShiftCloseV1::ShiftCloseV1() : SaxSerializerBase("ShiftClose")
{

}

void ShiftCloseV1::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "date", m_date);
}

void ShiftCloseV1::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "date", m_date);
}
