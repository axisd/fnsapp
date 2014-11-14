#ifndef _MONEY_OPERATION
#define _MONEY_OPERATION

// Main
#include "model_money_operation.h"
#include "sax_serializer_base.h"
#include "cfixedpointround.h"

// Qt
#include <QDateTime>

namespace SSCO
{
	class MoneyOperation : public ModelMoneyOperation, public SaxSerializerBase
	{
	public:
		MoneyOperation();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);
	};

	typedef boost::shared_ptr<MoneyOperation> MoneyOperationPtr;
}

#endif


	
