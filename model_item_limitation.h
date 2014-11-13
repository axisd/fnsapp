#ifndef _MODEL_ITEM_LIMITATION_H
#define _MODEL_ITEM_LIMITATION_H

#include "sax_serializer_base.h"

#include <QString>

/*
//	модель привязок ограничений к товарам	
*/
namespace SSCO
{
	struct ModelItemLimitation: public SaxSerializerBase
	{
		ModelItemLimitation();
		ModelItemLimitation(const QString& __item);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		QString m_item;
		QString m_limitation;
	};

	typedef boost::shared_ptr<ModelItemLimitation> ItemLimitationPtr;
}
#endif



