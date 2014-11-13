#include "shift_open.h"

using namespace SSCO;

ShiftOpen::ShiftOpen() :
	SaxSerializerBase("ShiftOpen")
{

}

void ShiftOpen::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "number", m_number);
	writeTag(__writer, "date", m_date);
	writeTag(__writer, "sale", m_sale);
	writeTag(__writer, "cancel",m_cancel );
	writeTag(__writer, "sale_fiscal", m_sale_fiscal);
	writeTag(__writer, "uploaded", m_uploaded);
}

void ShiftOpen::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "number", m_number);
	readTag(__reader, "date", m_date);
	readTag(__reader, "sale", m_sale);
	readTag(__reader, "cancel",m_cancel );
	readTag(__reader, "sale_fiscal", m_sale_fiscal);
	readTag(__reader, "uploaded", m_uploaded);
}
