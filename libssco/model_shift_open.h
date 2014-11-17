#ifndef _MODEL_SHIFT_OPEN
#define _MODEL_SHIFT_OPEN

#include "cfixedpointround.h"
#include <QDateTime>

namespace SSCO
{
	struct ModelShiftOpen 
	{
		unsigned int m_id;
		unsigned int m_number;
		QDateTime m_date;
		currency m_sale;
		currency m_cancel;
		currency m_sale_fiscal;
		bool m_uploaded;
	};
}
#endif
