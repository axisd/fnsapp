#ifndef RECEIPT_ITEM_TAX_H
#define RECEIPT_ITEM_TAX_H

// Main
#include "model_receipt_item_tax.h"
#include "sax_serializer_base.h"

namespace SSCO
{
    class ReceiptItemTax : public ModelReceiptItemTax, public SaxSerializerBase
    {
    public:
        ReceiptItemTax();

        //	SaxSerializerBase сохранение/чтение из xml
        void serialize (QXmlStreamWriter& __writer);
        void deserialize (QXmlStreamReader& __reader);
    };

    typedef boost::shared_ptr<ReceiptItemTax> ReceiptItemTaxPtr;
}

#endif // RECEIPT_ITEM_TAX_H
