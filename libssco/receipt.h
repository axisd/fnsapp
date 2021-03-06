#ifndef _RECEIPT_H
#define _RECEIPT_H

// models
#include "model_receipt.h"
#include "model_receipt_item.h"
#include "shift_open.h"
#include "model_receipt_payment_cash.h"
#include "model_receipt_payment_cashless.h"

#include "sax_serializer_base.h"

// boost QT
#include <boost/shared_ptr.hpp>
#include <QVector>

namespace SSCO
{
	class Receipt: public ModelReceipt, public SaxSerializerBase
	{
	public:	
		Receipt();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		void addItem (const boost::shared_ptr<ModelReceiptItem>& __item);

		const currency getTotal() const;
		const currency getTotalWithoutDiscounts() const;
		void setItemsPosition();

		QVector<ReceiptItemPtr> m_items;

		ReceiptPaymentCashPtr m_payment_cash;
		ReceiptPaymentCashlessPtr m_payment_cashless;

		unsigned int m_shift_number;
	};

	typedef boost::shared_ptr<Receipt> ReceiptPtr;
}

#endif