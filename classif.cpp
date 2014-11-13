#include "classif.h"

using namespace SSCO;

Classif::Classif() :
	SaxSerializerBase("Classif")
{

}

void Classif::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer,"id", m_id);

	writeOptional(__writer, "owner", m_owner);

	writeTag(__writer, "name", m_name);
	writeTag(__writer, "age_control", m_age_control);
	writeTag(__writer, "version", m_version);

	writeVector(__writer, "Limitations", m_limitations);
}

void Classif::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readOptional(__reader, "owner", m_owner );

	readTag(__reader, "name", m_name);
	readTag(__reader, "age_control", m_age_control);
	readTag(__reader, "version", m_version);

	readVector<ModelClassifLimitation>(__reader, "Limitations", m_limitations);
}
