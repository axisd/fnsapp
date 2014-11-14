#include "model_receipt_item_v1.h"

using namespace SSCO;

ModelReceiptItemV1::ModelReceiptItemV1() : SaxSerializerBase("ReceiptItem")
{

}

ModelReceiptItemV1::ModelReceiptItemV1(unsigned int __receipt) : SaxSerializerBase("ReceiptItem"),
	m_receipt(__receipt)
{

}

SSCO::ModelReceiptItemV1::ModelReceiptItemV1( const unsigned int __id,
	const unsigned int __receipt_id, 
	const ItemPtr __item, 
	const QString &__barcode, 
	quantity_t __quantity ) :
								m_id(__id),
								m_receipt(__receipt_id),
								m_item(__item->m_id),
								m_barcode(__barcode),
								m_name(__item->m_name),
                                m_type(ITEM_NORMAL),
								m_classif(__item->m_classif),
								m_quantity(__quantity),
								m_weighting(__item->m_weighting),
								m_price(__item->m_price),
								m_minprice(__item->m_minprice),
								m_total(__item->m_price * __quantity),
								//m_discount,
								m_measurement(__item->m_measure),
								m_measureprec(__item->m_measureprec),
								//m_position,
								SaxSerializerBase("ReceiptItem")
{

}

void ModelReceiptItemV1::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "receipt", m_receipt);
	writeTag(__writer, "item", m_item);

	writeOptional(__writer, "barcode", m_barcode);

	writeTag(__writer, "name", m_name);
    writeTag(__writer, "type", m_type);
	writeTag(__writer, "classif", m_classif);
	writeTag(__writer, "quantity", m_quantity);
	writeTag(__writer, "weighting", m_weighting);
	writeTag(__writer, "price", m_price);

	writeOptional(__writer, "minprice", m_minprice);
	writeOptional(__writer, "total", m_total);

	writeTag(__writer, "discount", m_discount);
	writeTag(__writer, "measurement", m_measurement);
	writeTag(__writer, "measureprec", m_measureprec);
	writeTag(__writer, "position", m_position);

    writeVector(__writer, "taxs", m_taxs);
	writeVector(__writer, "discounts", m_discounts);
}

void ModelReceiptItemV1::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "receipt", m_receipt);
	readTag(__reader, "item", m_item);

	readOptional(__reader, "barcode", m_barcode);

	readTag(__reader, "name", m_name);
    readTag(__reader, "type", m_type);
	readTag(__reader, "classif", m_classif);
	readTag(__reader, "quantity", m_quantity);
	readTag(__reader, "weighting", m_weighting);
	readTag(__reader, "price", m_price);

	readOptional(__reader, "minprice", m_minprice);
	readOptional(__reader, "total", m_total);

	readTag(__reader, "discount", m_discount);
	readTag(__reader, "measurement", m_measurement);
	readTag(__reader, "measureprec", m_measureprec);
	readTag(__reader, "position", m_position);

    readVector(__reader, "taxs", m_taxs);
	readVector(__reader, "discounts", m_discounts);
}

currency ModelReceiptItemV1::totalWithDisc()
{
	currency total(0);

	total += m_total.get();

	for (QVector<ReceiptItemDiscountPtr>::const_iterator discounts = m_discounts.begin(); 
		discounts != m_discounts.end();
		++discounts) 
	{
		if((*discounts)->m_value.get() < 0)
		{
			total += (*discounts)->m_value;	
		}
		else
		{
			total -= (*discounts)->m_value;	
		}
	}

	return total;
}

