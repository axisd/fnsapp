#ifndef _TIME_LIMITATION_H
#define _TIME_LIMITATION_H

#include "model_time_limitation.h"
#include "sax_serializer_base.h"

// Qt
#include <QMap>

/*
//	модель ограничений продаж товаров по времени
*/
namespace SSCO
{
	class TimeLimitation : public ModelTimeLimitation, public SaxSerializerBase
	{
	public:
		TimeLimitation();

		//	SaxSerializerBase сохранение/чтение из xml
		void serialize (QXmlStreamWriter& __writer);
		void deserialize (QXmlStreamReader& __reader);

		struct DaysTimes
		{
			short int m_day_from;
			short int m_day_to;
			QString m_time_from;
			QString m_time_to;
		};

		void check();

	private:
		QMap<unsigned char, QString> m_days_words;
		QMap<unsigned char, unsigned char> m_days;
		void check_range();
		void check_days();
		void initMDays();
	};

	typedef boost::shared_ptr<TimeLimitation> TimeLimitationPtr;
}
#endif