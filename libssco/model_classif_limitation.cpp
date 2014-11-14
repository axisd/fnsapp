#include "model_classif_limitation.h"

using namespace SSCO;

ModelClassifLimitation::ModelClassifLimitation() :
	SaxSerializerBase("ClassifLimitation")
{

}

ModelClassifLimitation::ModelClassifLimitation(const QString& __classif) :
	SaxSerializerBase("ClassifLimitation"),
	m_classif(__classif)
{

}
	
void ModelClassifLimitation::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "classif", m_classif);
	writeTag(__writer, "limitation", m_limitation);
}

void ModelClassifLimitation::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "classif", m_classif);
	readTag(__reader, "limitation", m_limitation);
}

