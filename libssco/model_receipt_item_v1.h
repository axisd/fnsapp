#ifndef _MODEL_RECEIPT_ITEM_V1_H
#define _MODEL_RECEIPT_ITEM_V1_H

// Main
#include "sax_serializer_base.h"
#include "item.h"
#include "model_receipt_item_discount.h"
#include "receipt_item_tax.h"

// Boost
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QString>

namespace SSCO
{
    struct ModelReceiptItemV1 : public SaxSerializerBase
	{
        enum TypeItem {
            ITEM_NORMAL = 0, // нормально заведенная позиция чека
            ITEM_CANCEL = 1, // аннулированная позция чека - не используется
            ITEM_VOID = 2 // сторнирующая позиция чека
        };

        ModelReceiptItemV1();
        ModelReceiptItemV1(unsigned int __receipt);
        ModelReceiptItemV1(const unsigned int __id,
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
        unsigned int m_type;
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

        // на самом деле налог вроде как один
        QVector<ReceiptItemTaxPtr> m_taxs;

		QVector<ReceiptItemDiscountPtr> m_discounts;
	};

    typedef boost::shared_ptr<ModelReceiptItemV1> ReceiptItemV1Ptr;
}

#endif
