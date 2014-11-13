#ifndef _MODEL_RECEIPT_ITEM_DISCOUNT_H_
#define _MODEL_RECEIPT_ITEM_DISCOUNT_H_

// Main
#include "sax_serializer_base.h"

// Boost
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QString>

namespace SSCO
{
	struct ModelReceiptItemDiscount : public SaxSerializerBase
	{
		ModelReceiptItemDiscount();
		ModelReceiptItemDiscount(const unsigned int __item);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		unsigned int m_item;
		unsigned int m_discount_type_id;
		QString m_name;
		currency m_value;
	};

	typedef boost::shared_ptr<ModelReceiptItemDiscount> ReceiptItemDiscountPtr;
}


#endif
