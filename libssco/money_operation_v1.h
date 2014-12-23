#ifndef _MONEY_OPERATION_V1
#define _MONEY_OPERATION_V1

#include "model_money_operation_v1.h"

#include "sax_serializer_base.h"

#include "cfixedpointround.h"

// Qt
#include <QDateTime>

namespace SSCO
{
	class MoneyOperationV1 : public ModelMoneyOperationV1, public SaxSerializerBase
	{
	public:
		MoneyOperationV1();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
	};

	typedef boost::shared_ptr<MoneyOperationV1> MoneyOperationV1Ptr;
}

#endif


	
