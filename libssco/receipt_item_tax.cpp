#include "receipt_item_tax.h"

using namespace SSCO;

ReceiptItemTax::ReceiptItemTax() : SaxSerializerBase("ReceiptItemTax")
{

}

void ReceiptItemTax::serialize(QXmlStreamWriter& __writer)
{
    writeOptional(__writer, "id", m_id);
    writeTag(__writer, "tax_group_id", m_tax_group_id);
    writeTag(__writer, "tax_id", m_tax_id);
    writeTag(__writer, "priority", m_priority);
    writeTag(__writer, "name", m_name);
    writeTag(__writer, "percent", m_percent);
    writeTag(__writer, "amount", m_amount);
}

void ReceiptItemTax::deserialize (QXmlStreamReader& __reader)
{
    readOptional(__reader, "id", m_id);
    readTag(__reader, "tax_group_id", m_tax_group_id);
    readTag(__reader, "tax_id", m_tax_id);
    readTag(__reader, "priority", m_priority);
    readTag(__reader, "name", m_name);
    readTag(__reader, "percent", m_percent);
    readTag(__reader, "amount", m_amount);
}
