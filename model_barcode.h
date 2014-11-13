#ifndef _MODEL_BARCODE_H
#define _MODEL_BARCODE_H

#include "sax_serializer_base.h"

#include "cfixedpointround.h"
#include <QString>

/*
//	Модель штрихкодов
*/
namespace SSCO
{
	struct ModelBarcode: public SaxSerializerBase
	{
		ModelBarcode();
		ModelBarcode(const QString& __parent_item);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		QString m_id;
		QString m_item;
		quantity_t m_quantity;
	};

	typedef boost::shared_ptr<ModelBarcode> BarcodePtr;
}
#endif
