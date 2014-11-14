#ifndef _RECEIPT_V1_H
#define _RECEIPT_V1_H

// models
#include "model_receipt_v1.h"
#include "model_receipt_item_v1.h"
#include "shift_open.h"
#include "model_receipt_payment_cash.h"
#include "model_receipt_payment_cashless.h"

#include "sax_serializer_base.h"

// boost QT
#include <boost/shared_ptr.hpp>
#include <QVector>

namespace SSCO
{
    class ReceiptV1: public ModelReceiptV1, public SaxSerializerBase
	{
	public:	
        ReceiptV1();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

        void addItem (const boost::shared_ptr<ModelReceiptItemV1>& __item);

		const currency getTotal() const;
		const currency getTotalWithoutDiscounts() const;
		void setItemsPosition();

        QVector<ReceiptItemV1Ptr> m_items;

		ReceiptPaymentCashPtr m_payment_cash;
		ReceiptPaymentCashlessPtr m_payment_cashless;

		unsigned int m_shift_number;
	};

    typedef boost::shared_ptr<ReceiptV1> ReceiptV1Ptr;
}

#endif
