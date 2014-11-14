#ifndef _MODEL_TIME_LIMITATION_H
#define _MODEL_TIME_LIMITATION_H

#include <boost/optional.hpp>
#include <QString>
#include <QTime>

/*
//	модель ограничений продаж товаров по времени
*/
namespace SSCO
{
	struct ModelTimeLimitation
	{
		ModelTimeLimitation() : m_id(0), m_time_type(0), m_version(0)  {};

		unsigned int m_id;
		QString m_name;
		short int m_time_type;
		boost::optional<short> m_day_from;
		boost::optional<short> m_day_to;
		boost::optional<QTime> m_time_from;
		boost::optional<QTime> m_time_to;
		boost::optional<bool> m_monday;
		boost::optional<bool> m_tuesday;
		boost::optional<bool> m_wednesday;
		boost::optional<bool> m_thursday;
		boost::optional<bool> m_friday;
		boost::optional<bool> m_saturday;
		boost::optional<bool> m_sunday;
		unsigned int m_version;

		enum Days 
		{
			SUNDAY = 0,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY			
		};

		enum TimeTypes
		{
			BY_DAYS = 0,
			BY_RANGE
		};
	};
}
#endif