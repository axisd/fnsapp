#ifndef _CLASSIF
#define  _CLASSIF

#include "model_classif.h"
#include "model_classif_limitation.h"

#include "sax_serializer_base.h"

#include <QVector>

namespace SSCO
{
	class Classif: public ModelClassif, public SaxSerializerBase
	{
	public:	
		Classif();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		QVector<ClassifLimitationPtr> m_limitations;
	};

	typedef boost::shared_ptr<Classif> ClassifPtr;
}
#endif