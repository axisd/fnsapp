#include "model_item_limitation.h"

using namespace SSCO;

ModelItemLimitation::ModelItemLimitation() :
	SaxSerializerBase("ItemLimitation")
{

}

ModelItemLimitation::ModelItemLimitation(const QString& __item) :
	SaxSerializerBase("ItemLimitation"),
	m_item(__item)
{

}

void ModelItemLimitation::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "item", m_item);
	writeTag(__writer, "limitation", m_limitation);
}

void ModelItemLimitation::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "item", m_item);
	readTag(__reader, "limitation", m_limitation);
}



