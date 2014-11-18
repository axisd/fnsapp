#include "model_receipt_payment_cash.h"

#include "liblogger/cdebug.h"

using namespace SSCO;

ModelReceiptPaymentCash::ModelReceiptPaymentCash() : SaxSerializerBase("ReceiptPaymentCash"),
	m_amount(0),
	m_amount_with_change(0)
{

}

ModelReceiptPaymentCash::ModelReceiptPaymentCash(unsigned int __receipt) : SaxSerializerBase("ReceiptPaymentCash"),
	m_receipt(__receipt),
	m_amount(0),
	m_amount_with_change(0)
{

}

void ModelReceiptPaymentCash::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "receipt", m_receipt);
	writeTag(__writer, "amount", m_amount);
	writeTag(__writer, "amount_with_change", m_amount_with_change);
}

void ModelReceiptPaymentCash::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "receipt", m_receipt);
    LOG_MESSAGE(logger::t_info, "main", "ModelReceiptPaymentCash::deserialize receipt");

	readTag(__reader, "amount", m_amount);
    LOG_MESSAGE(logger::t_info, "main", "ModelReceiptPaymentCash::deserialize amount");

	readTag(__reader, "amount_with_change", m_amount_with_change);
    LOG_MESSAGE(logger::t_info, "main", "ModelReceiptPaymentCash::deserialize amount_with_change");
}
