#ifndef _MODEL_SHIFT_CLOSE
#define _MODEL_SHIFT_CLOSE

#include <boost/optional.hpp>
#include <QString>
#include <QDateTime>

namespace SSCO
{
	struct ModelShiftClose
	{
		unsigned int m_id;
		QDateTime m_date;
		boost::optional<unsigned int> m_kkm_shift_number;
		QString m_kkm_serial_number;
		QString m_kkm_registration_number;
		QString m_kkm_owner_number;
		QString m_eklz_number;
		boost::optional<QDate>	m_eklz_date_activate;
		boost::optional<bool> m_eklz_fast_full;
		QString m_kkm_model_name;
		bool m_uploaded;
	};
}
#endif