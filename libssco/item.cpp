#include "item.h"

using namespace SSCO;

Item::Item() :
	SaxSerializerBase("Item")
{

}
	
void Item::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "classif", m_classif);
	writeTag(__writer, "name", m_name);
	writeTag(__writer, "price", m_price);
	writeOptional(__writer, "minprice", m_minprice);
	writeTag(__writer, "measure", m_measure);
	writeTag(__writer, "measureprec", m_measureprec);
	writeTag(__writer, "weighting", m_weighting);
	writeTag(__writer, "age_control", m_age_control);
	writeTag(__writer, "version", m_version);

	writeVector(__writer, "Barcodes", m_barcodes);
	writeVector(__writer, "Limitations", m_limitations);
}

void Item::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "classif", m_classif);
	readTag(__reader, "name", m_name);
	readTag(__reader, "price", m_price);
	readOptional(__reader, "minprice", m_minprice);
	readTag(__reader, "measure", m_measure);
	readTag(__reader, "measureprec", m_measureprec);
	readTag(__reader, "weighting", m_weighting);
	readTag(__reader, "age_control", m_age_control);
	readTag(__reader, "version", m_version);

	readVector<ModelBarcode>(__reader, "Barcodes", m_barcodes);
	readVector<ModelItemLimitation>(__reader, "Limitations", m_limitations);
}
