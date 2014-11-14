#ifndef _MODEL_RECEIPT_H
#define _MODEL_RECEIPT_H

#include <boost/optional.hpp>
#include <QDateTime>

namespace SSCO
{
	struct ModelReceipt
	{
		unsigned int m_id;
		unsigned int m_global_number;
		unsigned int m_local_number;
		unsigned int m_shift_open;
		QDateTime m_open_datetime;
		boost::optional<QDateTime> m_close_datetime;
		boost::optional<QString> m_card_number;
		bool m_uploaded;
	};
}
#endif