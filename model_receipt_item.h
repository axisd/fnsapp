#ifndef _MODEL_RECEIPT_ITEM
#define _MODEL_RECEIPT_ITEM

// Main
#include "sax_serializer_base.h"
#include "item.h"
#include "model_receipt_item_discount.h"

// Boost
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QString>

namespace SSCO
{
	struct ModelReceiptItem : public SaxSerializerBase
	{
		ModelReceiptItem();
		ModelReceiptItem(unsigned int __receipt);
		ModelReceiptItem(const unsigned int __id, 
							const unsigned int __receipt_id, 
							const ItemPtr __item,
							const QString &__barcode,
							quantity_t m_quantity);

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
		currency totalWithDisc();

		unsigned int m_id;
		unsigned int m_receipt;
		QString m_item;
		boost::optional<QString> m_barcode;
		QString m_name;
		QString m_classif;
		quantity_t m_quantity;
		bool m_weighting;
		currency m_price;
		boost::optional<currency> m_minprice;
		boost::optional<currency> m_total;
		currency m_discount;
		QString m_measurement;
		int m_measureprec;
		int m_position;

		QVector<ReceiptItemDiscountPtr> m_discounts;
	};

	typedef boost::shared_ptr<ModelReceiptItem> ReceiptItemPtr;
}


#endif
