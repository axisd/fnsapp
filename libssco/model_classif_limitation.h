#ifndef _MODEL_CLASSIF_LIMITATION_H
#define _MODEL_CLASSIF_LIMITATION_H

#include "sax_serializer_base.h"

#include <QString>

/*
//	модель привязок ограничений к узлам классификатора
*/
namespace SSCO
{
	struct ModelClassifLimitation: public SaxSerializerBase
	{
		ModelClassifLimitation();
		ModelClassifLimitation(const QString& __classif);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		QString m_classif;
		QString m_limitation;
	};

	typedef boost::shared_ptr<ModelClassifLimitation> ClassifLimitationPtr;
}
#endif