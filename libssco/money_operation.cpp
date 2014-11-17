#include "money_operation.h"

using namespace SSCO;

MoneyOperation::MoneyOperation() : SaxSerializerBase("MoneyOperation")
{

}

void MoneyOperation::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "global_number", m_global_number);
	writeTag(__writer, "local_number", m_local_number);
	writeTag(__writer, "shift_open", m_shift_open);
	writeTag(__writer, "date", m_date);
	writeTag(__writer, "type", m_type);
	writeTag(__writer, "amount", m_amount);
	writeTag(__writer, "amount_before", m_amount_before);
	writeTag(__writer, "uploaded", m_uploaded);
}

void MoneyOperation::deserialize(QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "global_number", m_global_number);
	readTag(__reader, "local_number", m_local_number);
	readTag(__reader, "shift_open", m_shift_open);
	readTag(__reader, "date", m_date);
	readTag(__reader, "type", m_type);
	readTag(__reader, "amount", m_amount);
	readTag(__reader, "amount_before", m_amount_before);
	readTag(__reader, "uploaded", m_uploaded);
}



	
