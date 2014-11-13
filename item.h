#ifndef _ITEM
#define _ITEM

#include "model_item.h"
#include "model_item_limitation.h"
#include "classif.h"

#include "time_limitation.h"
#include "model_barcode.h"

#include "sax_serializer_base.h"

#include <QVector>

namespace SSCO
{
	class Item: public ModelItem, public SaxSerializerBase
	{
	public:
		Item();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		QVector<BarcodePtr> m_barcodes;
		QVector<ItemLimitationPtr> m_limitations;
		QVector<TimeLimitationPtr> m_time_limitations;
		QVector<ClassifLimitationPtr> m_classif_limitations;
	};

	typedef boost::shared_ptr<Item> ItemPtr;
}
#endif