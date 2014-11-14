#include "model_receipt_item_discount.h"

using namespace SSCO;

ModelReceiptItemDiscount::ModelReceiptItemDiscount() : SaxSerializerBase("ReceiptItemDiscount")
{

}

ModelReceiptItemDiscount::ModelReceiptItemDiscount(const unsigned int __item) : m_item(__item), SaxSerializerBase("ReceiptItemDiscount")
{

}

void ModelReceiptItemDiscount::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "item", m_item);
	writeTag(__writer, "discount_type_id", m_discount_type_id);
	writeTag(__writer, "name", m_name);
	writeTag(__writer, "value", m_value);
}

void ModelReceiptItemDiscount::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "item", m_item);
	readTag(__reader, "discount_type_id", m_discount_type_id);
	readTag(__reader, "name", m_name);
	readTag(__reader, "value", m_value);
}