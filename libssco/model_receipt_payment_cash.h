#ifndef _MODEL_RECEIPT_PAYMENT_CASH
#define _MODEL_RECEIPT_PAYMENT_CASH

#include "sax_serializer_base.h"

namespace SSCO
{
	struct ModelReceiptPaymentCash : public SaxSerializerBase
	{
		ModelReceiptPaymentCash();
		ModelReceiptPaymentCash(unsigned int __receipt);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		unsigned int m_receipt;
		currency m_amount;
		currency m_amount_with_change;
	};

	typedef boost::shared_ptr<ModelReceiptPaymentCash> ReceiptPaymentCashPtr;
}
#endif

