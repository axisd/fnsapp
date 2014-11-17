#ifndef MODEL_RECEIPT_ITEM_TAX_H
#define MODEL_RECEIPT_ITEM_TAX_H

// Main
#include "cfixedpointround.h"

// Boost
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QString>

namespace SSCO
{
    struct ModelReceiptItemTax
    {
        boost::optional<unsigned long long> m_id;
        int m_tax_group_id;
        int m_tax_id;
        int m_priority;
        QString m_name;
        QString m_percent;
        currency m_amount;
    };
}

#endif // MODEL_RECEIPT_ITEM_TAX_H
