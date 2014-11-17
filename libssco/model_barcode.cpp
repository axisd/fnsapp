#include "model_barcode.h"

using namespace SSCO;

ModelBarcode::ModelBarcode() :
	SaxSerializerBase("Barcode")
{

}

ModelBarcode::ModelBarcode(const QString& __item) :
	SaxSerializerBase("Barcode"),
	m_item(__item)
{

}

void ModelBarcode::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "item", m_item);
	writeTag(__writer, "quantity", m_quantity);
}

void ModelBarcode::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "item", m_item);
	readTag(__reader, "quantity", m_quantity);
}