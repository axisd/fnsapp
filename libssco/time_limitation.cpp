
#include "time_limitation.h"

// Boost
#include <boost/date_time.hpp>

using namespace SSCO;

TimeLimitation::TimeLimitation() :
	SaxSerializerBase("TimeLimitation")
{
	m_days[boost::date_time::Sunday] = 0;
	m_days[boost::date_time::Monday] = 0;
	m_days[boost::date_time::Tuesday] = 0;
	m_days[boost::date_time::Wednesday] = 0;
	m_days[boost::date_time::Thursday] = 0;
	m_days[boost::date_time::Friday]= 0;
	m_days[boost::date_time::Saturday] = 0;
	m_days_words[boost::date_time::Sunday] = QObject::tr("Воскресенье");
	m_days_words[boost::date_time::Monday] = QObject::tr("Понедельник");
	m_days_words[boost::date_time::Tuesday] = QObject::tr("Вторник");
	m_days_words[boost::date_time::Wednesday] = QObject::tr("Среда");
	m_days_words[boost::date_time::Thursday] = QObject::tr("Четверг");
	m_days_words[boost::date_time::Friday]= QObject::tr("Пятница");
	m_days_words[boost::date_time::Saturday] = QObject::tr("Суббота");
}

void TimeLimitation::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "name", m_name);
	writeTag(__writer, "time_type", m_time_type);

	writeOptional(__writer, "day_from", m_day_from);
	writeOptional(__writer, "day_to", m_day_to);

	writeOptional(__writer, "time_from", m_time_from);
	writeOptional(__writer, "time_to", m_time_to);
	
	writeOptional(__writer, "monday", m_monday);
	writeOptional(__writer, "tuesday", m_tuesday);
	writeOptional(__writer, "wednesday", m_wednesday);
	writeOptional(__writer, "thursday", m_thursday);
	writeOptional(__writer, "friday", m_friday);
	writeOptional(__writer, "saturday", m_saturday);
	writeOptional(__writer, "sunday", m_sunday);

	writeTag(__writer, "version", m_version);
}

void TimeLimitation::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "name", m_name);
	readTag(__reader, "time_type", m_time_type);

	readOptional(__reader, "day_from", m_day_from);
	readOptional(__reader, "day_to", m_day_to);

	readOptional(__reader, "time_from", m_time_from);
	readOptional(__reader, "time_to", m_time_to);

	readOptional(__reader, "monday", m_monday);
	readOptional(__reader, "tuesday", m_tuesday);
	readOptional(__reader, "wednesday", m_wednesday);
	readOptional(__reader, "thursday", m_thursday);
	readOptional(__reader, "friday", m_friday);
	readOptional(__reader, "saturday", m_saturday);
	readOptional(__reader, "sunday", m_sunday);

	readTag(__reader, "version", m_version);
}

/*const QVector<TimeLimitation::DaysTimes> TimeLimitation::getLimits() const
{
	QVector<DaysTimes> v_dt;

	if(m_time_type == ModelTimeLimitation::BY_RANGE)
	{
		DaysTimes dt;
		dt.m_day_from = ( *m_day_from == 0 ? 6 : *m_day_from - 1 );
		dt.m_day_to = ( *m_day_to == 0 ? 6 : *m_day_to - 1 );
		dt.m_time_from = getTime(*m_time_from);
		dt.m_time_to = getTime(*m_time_to);
		v_dt.push_back(dt);
	}
	else
	{
		if(m_monday)
		{
			if(*m_monday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::MONDAY;
				dt.m_day_to = ModelTimeLimitation::MONDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_tuesday)
		{
			if(*m_tuesday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::TUESDAY;
				dt.m_day_to = ModelTimeLimitation::TUESDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_wednesday)
		{
			if(*m_wednesday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::WEDNESDAY;
				dt.m_day_to = ModelTimeLimitation::WEDNESDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_thursday)
		{
			if(*m_thursday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::THURSDAY;
				dt.m_day_to = ModelTimeLimitation::THURSDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_friday)
		{
			if(*m_friday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::FRIDAY;
				dt.m_day_to = ModelTimeLimitation::FRIDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_saturday)
		{
			if(*m_saturday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::SATURDAY;
				dt.m_day_to = ModelTimeLimitation::SATURDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}

		if(m_sunday)
		{
			if(*m_sunday)
			{
				DaysTimes dt;
				dt.m_day_from = ModelTimeLimitation::SUNDAY;
				dt.m_day_to = ModelTimeLimitation::SUNDAY;
				dt.m_time_from = getTime(*m_time_from);
				dt.m_time_to = getTime(*m_time_to);
				v_dt.push_back(dt);
			}
		}
	}

	return v_dt;
}

QString TimeLimitation::getTime( const QTime &__time ) const
{
	return QString("%1").arg(__time.hour() * 3600 + __time.minute() * 60 + __time.second());
}*/

void TimeLimitation::check_range()
{
	unsigned short cur_day = static_cast<unsigned short>(boost::posix_time::second_clock::local_time().date().day_of_week());
	QMap<unsigned char, unsigned char> m_range_days;
	QString errSTring = QObject::tr("Ограничение по периоду. [%1 %2] - [%3 %4]")
		.arg(m_days_words[m_day_from.get()])
		.arg(m_time_from.get().toString(QObject::tr("hh:mm")))
		.arg(m_days_words[m_day_to.get()])
		.arg(m_time_to.get().toString(QObject::tr("hh:mm")));
	if(m_day_from.get() > m_day_to.get())
	{
		// day from the next week
		if(cur_day > m_day_from.get() || cur_day < m_day_to.get())
		{
			throw std::runtime_error(errSTring.toStdString());
		}
		else if(cur_day == m_day_from.get())
		{
			if(QTime::currentTime() >= m_time_from.get())
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
		else if(cur_day == m_day_to.get())
		{
			if(QTime::currentTime() <= m_time_to.get())
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
	}
	else if(m_day_from.get() < m_day_to.get())
	{
		// day in this week
		if(cur_day > m_day_from.get() && cur_day < m_day_to.get())
		{
			throw std::runtime_error(errSTring.toStdString());
		}
		else if(cur_day == m_day_from.get())
		{
			if(QTime::currentTime() >= m_time_from.get())
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
		else if(cur_day == m_day_to.get())
		{
			if(QTime::currentTime() <= m_time_to.get())
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
	}
	else if(m_day_from.get() == m_day_to.get())
	{
		// day from the next week, the same day
		if(m_time_from.get() > m_time_to.get())
		{
			// almost the whole week
			if(cur_day == m_day_to.get())
			{
				if(QTime::currentTime() >= m_time_from.get() || QTime::currentTime() <= m_time_to.get())
				{
					throw std::runtime_error(errSTring.toStdString());
				}
			}
			else
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
		else
		{
			// just this day
			if((m_day_from.get() == cur_day) && (QTime::currentTime() >= m_time_from.get() && QTime::currentTime() <= m_time_to.get()))
			{
				throw std::runtime_error(errSTring.toStdString());
			}
		}
	}
}

void TimeLimitation::check_days()
{
	unsigned char cur_day = static_cast<unsigned short>(boost::posix_time::second_clock::local_time().date().day_of_week());
	if(m_time_from.get() >= m_time_to.get())
	{	
		QMap<unsigned char, unsigned char> m_next_days;
		for(int di =boost::date_time::Sunday; di != (boost::date_time::Saturday + 1); di++)
		{
			if(m_days[di] == 1)
			{
				if(di != boost::date_time::Saturday)
				{
					m_next_days[di+1] = 1;
				}
				else
				{
					m_next_days[boost::date_time::Sunday] = 1;
				}		
			}
		}

		if(m_days[cur_day] == 1 && QTime::currentTime() >= m_time_from.get())
		{
			throw std::runtime_error(QObject::tr("Ограничение по времени с %1 до %2 следующего дня")
				.arg(m_time_from.get().toString("hh:mm"))
				.arg(m_time_to.get().toString("hh:mm"))
				.toStdString());
		}
		if(m_next_days[cur_day] == 1 && QTime::currentTime() < m_time_to.get())
		{
			throw std::runtime_error(QObject::tr("Ограничение по времени с %1 до %2")
				.arg(m_time_from.get().toString("hh:mm"))
				.arg(m_time_to.get().toString("hh:mm"))
				.toStdString());
		}
	}
	else
	{
		if(m_days[cur_day] == 1 && QTime::currentTime() >= m_time_from.get() && QTime::currentTime() < m_time_to.get())
		{
			throw std::runtime_error(QObject::tr("Ограничение по времени с %1 до %2")
				.arg(m_time_from.get().toString("hh:mm"))
				.arg(m_time_to.get().toString("hh:mm"))
				.toStdString());
		}
	}
}

void SSCO::TimeLimitation::check()
{
	initMDays();
	if(m_time_type == BY_RANGE)
	{
		check_range();
	}
	else
	{
		check_days();
	}
}

void SSCO::TimeLimitation::initMDays()
{
	if(m_monday && m_monday.get())
	{
		m_days[boost::date_time::Monday] = 1;
	}
	if(m_tuesday && m_tuesday.get())
	{
		m_days[boost::date_time::Tuesday] = 1;
	}
	if(m_wednesday && m_wednesday.get())
	{
		m_days[boost::date_time::Wednesday] = 1;
	}
	if(m_thursday&& m_thursday.get())
	{
		m_days[boost::date_time::Thursday] = 1;
	}
	if(m_friday&& m_friday.get())
	{
		m_days[boost::date_time::Friday] = 1;
	}
	if(m_saturday&& m_saturday.get())
	{
		m_days[boost::date_time::Saturday] = 1;
	}
	if(m_sunday&& m_sunday.get())
	{
		m_days[boost::date_time::Sunday] = 1;
	}
}
