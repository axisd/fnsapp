#include "receipt_v1.h"

#include "liblogger/cdebug.h"

using namespace SSCO;

ReceiptV1::ReceiptV1() :
	SaxSerializerBase("Receipt")
{

}

void ReceiptV1::serialize(QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "global_number", m_global_number);
	writeTag(__writer, "local_number", m_local_number);
	writeTag(__writer, "shift_open", m_shift_open);
    writeTag(__writer, "type", m_type);
    writeTag(__writer, "stock_name", m_stock_name);
	writeTag(__writer, "open_datetime", m_open_datetime);
	writeOptional(__writer, "close_datetime", m_close_datetime );
	writeOptional(__writer, "card_number", m_card_number );
    writeTag(__writer, "login_id", m_login_id);
    writeTag(__writer, "login_name", m_login_name);
	writeTag(__writer, "uploaded", m_uploaded);

	writeVector(__writer, "ReceiptItems", m_items);
	writeVector(__writer, "PaymentCash", m_payment_cash);
	writeVector(__writer, "PaymentCashless", m_payment_cashless);
}

void ReceiptV1::deserialize(QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize id: %1").arg(m_id));

	readTag(__reader, "global_number", m_global_number);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize global_number: %1").arg(m_global_number));

	readTag(__reader, "local_number", m_local_number);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize local_number: %1").arg(m_local_number));

	readTag(__reader, "shift_open", m_shift_open);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize shift_open: %1").arg(m_shift_open));

    readTag(__reader, "type", m_type);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize type: %1").arg(m_type));

    readTag(__reader, "stock_name", m_stock_name);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize stock_name: %1").arg(m_stock_name));

    readTag(__reader, "open_datetime", m_open_datetime);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize open_datetime: %1").arg(m_open_datetime.toString("ddMMyyyy hhmmss")));

	readOptional(__reader, "close_datetime", m_close_datetime );
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize close_datetime: %1")
//                .arg((m_close_datetime ? (*m_close_datetime).toString("ddMMyyyy hhmmss") : "ne zadan")));

	readOptional(__reader, "card_number", m_card_number );
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize card_number: %1")
//                .arg((m_card_number ? (*m_card_number) : "ne zadan")));

    readTag(__reader, "login_id", m_login_id);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize login_id: %1").arg(m_login_id));

    readTag(__reader, "login_name", m_login_name);
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize login_name: %1").arg(m_login_name));

	readTag(__reader, "uploaded", m_uploaded);	
//    LOG_MESSAGE(logger::t_info, "main", QString("ReceiptV1::deserialize uploaded: %1").arg(m_uploaded));


    readVector<ModelReceiptItemV1>(__reader, "ReceiptItems", m_items);
//    LOG_MESSAGE(logger::t_info, "main", "ReceiptV1::deserialize ReceiptItems");

	readVector<ModelReceiptPaymentCash>(__reader, "PaymentCash", m_payment_cash);
//    LOG_MESSAGE(logger::t_info, "main", "ReceiptV1::deserialize PaymentCash");

	readVector<ModelReceiptPaymentCashless>(__reader, "PaymentCashless", m_payment_cashless);
//    LOG_MESSAGE(logger::t_info, "main", "ReceiptV1::deserialize PaymentCashless");

}

void ReceiptV1::addItem (const boost::shared_ptr<ModelReceiptItemV1> &__item)
{
	m_items.push_back(__item);
}

const currency SSCO::ReceiptV1::getTotal() const
{
	currency total(0);

    for (QVector<ReceiptItemV1Ptr>::const_iterator item = m_items.begin(); item != m_items.end(); ++item)
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

const currency SSCO::ReceiptV1::getTotalWithoutDiscounts() const
{
	currency total(0);

    for (QVector<ReceiptItemV1Ptr>::const_iterator item = m_items.begin(); item != m_items.end(); ++item)
	{
		total += (*item)->m_total.get();
	}

	return total;
}
void SSCO::ReceiptV1::setItemsPosition()
{
    QVector<ReceiptItemV1Ptr>::const_iterator i;
	int pos(1);
	for (i = m_items.begin(); i != m_items.end(); ++i)
	{
		(*i)->m_position = pos;
		pos++;
	}
}
