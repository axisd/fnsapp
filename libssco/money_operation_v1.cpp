#include "money_operation_v1.h"

using namespace SSCO;

MoneyOperationV1::MoneyOperationV1() : SaxSerializerBase("MoneyOperation")
{

}

void MoneyOperationV1::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "date", m_date);
	writeTag(__writer, "type", m_type);
	writeTag(__writer, "amount", m_amount);
}

void MoneyOperationV1::deserialize(QXmlStreamReader& __reader)
{
	readTag(__reader, "date", m_date);
	readTag(__reader, "type", m_type);
	readTag(__reader, "amount", m_amount);
}
