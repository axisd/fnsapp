#ifndef _MODEL_MONEY_OPERATION
#define _MODEL_MONEY_OPERATION

// misc
#include "cfixedpointround.h"

// Qt
#include <QDateTime>

namespace SSCO
{
	struct ModelMoneyOperation
	{

		enum OperationType { PAID_IN = 0, PAID_OUT };

		unsigned int m_id;
		unsigned int m_global_number;
		unsigned int m_local_number;
		unsigned int m_shift_open;
		QDateTime m_date;
		unsigned int m_type;
		currency m_amount;
		currency m_amount_before;
		bool m_uploaded;
	};
}

#endif


	
