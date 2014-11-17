#include "model_receipt_payment_cashless.h"

using namespace SSCO;

ModelReceiptPaymentCashless::ModelReceiptPaymentCashless() : SaxSerializerBase("ReceiptPaymentCashless")
{
	
}

ModelReceiptPaymentCashless::ModelReceiptPaymentCashless(unsigned int __receipt) : SaxSerializerBase("ReceiptPaymentCashless"),
	m_receipt(__receipt) 
{
}

void ModelReceiptPaymentCashless::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "receipt", m_receipt);
	writeTag(__writer, "efts", m_efts);
	writeTag(__writer, "amount", m_amount);

	writeOptional(__writer, "card_number", m_card_number);
	writeOptional(__writer, "auth_code", m_auth_code);
	writeOptional(__writer, "ref_number", m_ref_number);
}

void ModelReceiptPaymentCashless::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "receipt", m_receipt);
	readTag(__reader, "efts", m_efts);
	readTag(__reader, "amount", m_amount);

	readOptional(__reader, "card_number", m_card_number);
	readOptional(__reader, "auth_code", m_auth_code);
	readOptional(__reader, "ref_number", m_ref_number);
}


