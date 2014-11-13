#include "receipt.h"

using namespace SSCO;

Receipt::Receipt() :
	SaxSerializerBase("Receipt")
{

}

void Receipt::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "global_number", m_global_number);
	writeTag(__writer, "local_number", m_local_number);
	writeTag(__writer, "shift_open", m_shift_open);
	writeTag(__writer, "open_datetime", m_open_datetime);
	writeOptional(__writer, "close_datetime", m_close_datetime );
	writeOptional(__writer, "card_number", m_card_number );
	writeTag(__writer, "uploaded", m_uploaded);

	writeVector(__writer, "ReceiptItems", m_items);

	if (m_payment_cash)
	{
		m_payment_cash->write(__writer);
	}

	if (m_payment_cashless)
	{
		m_payment_cashless->write(__writer);
	}	
}

void Receipt::deserialize(QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "global_number", m_global_number);
	readTag(__reader, "local_number", m_local_number);
	readTag(__reader, "shift_open", m_shift_open);
	readTag(__reader, "open_datetime", m_open_datetime);
	readOptional(__reader, "close_datetime", m_close_datetime );
	readOptional(__reader, "card_number", m_card_number );
	readTag(__reader, "uploaded", m_uploaded);	

	readVector<ModelReceiptItem>(__reader, "ReceiptItems", m_items);

	readObject<ModelReceiptPaymentCash>(__reader, "ReceiptPaymentCash", m_payment_cash);
	readObject<ModelReceiptPaymentCashless>(__reader, "ReceiptPaymentCashless", m_payment_cashless);
}

void Receipt::addItem (const boost::shared_ptr<ModelReceiptItem>& __item)
{
	m_items.push_back(__item);
}

const currency SSCO::Receipt::getTotal() const
{
	currency total(0);

	for (QVector<ReceiptItemPtr>::const_iterator item = m_items.begin(); item != m_items.end(); ++item)
	{
		total += (*item)->m_total.get();
		
		for (QVector<ReceiptItemDiscountPtr>::const_iterator discounts = (*item)->m_discounts.begin(); 
			discounts != (*item)->m_discounts.end();
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
	}

	return total;
}

const currency SSCO::Receipt::getTotalWithoutDiscounts() const
{
	currency total(0);

	for (QVector<ReceiptItemPtr>::const_iterator item = m_items.begin(); item != m_items.end(); ++item)
	{
		total += (*item)->m_total.get();
	}

	return total;
}
void SSCO::Receipt::setItemsPosition()
{
	QVector<ReceiptItemPtr>::const_iterator i;
	int pos(1);
	for (i = m_items.begin(); i != m_items.end(); ++i)
	{
		(*i)->m_position = pos;
		pos++;
	}
}
