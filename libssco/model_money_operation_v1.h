#ifndef _MODEL_MONEY_OPERATION_V1
#define _MODEL_MONEY_OPERATION_V1

#include "cfixedpointround.h"

// Qt
#include <QDateTime>

namespace SSCO
{
	struct ModelMoneyOperationV1
	{

		enum OperationType { PAID_IN = 0, PAID_OUT };

		QDateTime m_date;
		unsigned int m_type;
		currency m_amount;
	};
}

#endif
