#ifndef _MODEL_RECEIPT_PAYMENT_CASHLESS
#define _MODEL_RECEIPT_PAYMENT_CASHLESS

#include "sax_serializer_base.h"

#include <boost/optional.hpp>
#include <QString>

namespace SSCO
{
	struct ModelReceiptPaymentCashless: public SaxSerializerBase
	{
		ModelReceiptPaymentCashless();
		ModelReceiptPaymentCashless(unsigned int __receipt);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		unsigned int m_receipt;
		unsigned int m_efts;
		currency m_amount;
		boost::optional<QString> m_card_number;
		boost::optional<QString> m_auth_code;
		boost::optional<QString> m_ref_number;
	};

	typedef boost::shared_ptr<ModelReceiptPaymentCashless> ReceiptPaymentCashlessPtr;
}

#endif

