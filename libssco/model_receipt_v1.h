#ifndef _MODEL_RECEIPT_H
#define _MODEL_RECEIPT_H

#include <boost/optional.hpp>
#include <QDateTime>

namespace SSCO
{
    struct ModelReceiptV1
	{
        enum TypeReceipt {
            ///продажа
            HEADER_SALE = 0,
            ///возврат
            HEADER_RETURN = 1,
            ///возврат по чеку
            HEADER_RETURN_BY_RECEIPT = 4,
            ///восстановленный после откладывания чек
            HEADER_POP = 5,
            ///чек на инвентаризацию
            HEADER_STOCK_COUNT = 7
        };

		unsigned int m_id;
		unsigned int m_global_number;
		unsigned int m_local_number;
		unsigned int m_shift_open;
        unsigned int m_type;
		QDateTime m_open_datetime;
		boost::optional<QDateTime> m_close_datetime;
		boost::optional<QString> m_card_number;
		bool m_uploaded;
	};
}
#endif
