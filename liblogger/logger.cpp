#include "logger.h"

// Qt
#include <QDateTime>
#include <QDir>
#include <QMutexLocker>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QThread>

#ifndef NDEBUG
#include <iostream>
#endif

#if defined(_WIN32)
#include <Windows.h>
#endif

#ifndef NDEBUG
logger::string_list_t logger::m_skipped;
#endif

static QMutex mutex;

logger_wrapper::logger_wrapper(const logger& __log, const QString& __entry)
	: m_log(__log), m_entry(__entry), m_enabled(__log.enabled(__entry))
{
}

void logger_wrapper::info(const QString& text)
{
	if (m_enabled)
	{
		m_log.info(m_entry, text);
	}
}

void logger_wrapper::warning(const QString& text)
{
	if (m_enabled)
	{
		m_log.warning(m_entry, text);
	}
}

void logger_wrapper::debug(const QString& text)
{
	if (m_enabled)
	{
		m_log.debug(m_entry, text);
	}
}

void logger_wrapper::fatal(const QString& text)
{
	if (m_enabled)
	{
		m_log.fatal(m_entry, text);
	}
}

bool logger_wrapper::enabled() const
{
	return m_enabled;
}

logger::logger(const QString& __filename_format, const QString& __prefix_format)
	: m_filename_format(__filename_format)
	, m_prefix_format(__prefix_format)
{
}

void logger::set_format(const QString& __filename_format, const QString& __prefix_format)
{
	m_filename_format = __filename_format;
	m_prefix_format = __prefix_format;
}

void logger::fill_entries(const QString& Param)
{
	if (Param.isEmpty())
	{
		enabled_entries.clear();
	}
	else
	{
		enabled_entries.unite(Param.split(";", QString::SkipEmptyParts).toSet());
	}
}

void logger::add_entries(const QString& Param)
{
	if (!Param.isEmpty())
	{
		QString entry;
		Q_FOREACH(entry, Param.split(";", QString::SkipEmptyParts))
		{
			enabled_entries.insert(entry);
			disabled_entries.remove(entry);
		}
	}
}

void logger::remove_entries(const QString& Param)
{
	if (!Param.isEmpty())
	{
		QString entry;
		Q_FOREACH(entry, Param.split(";", QString::SkipEmptyParts))
		{
			enabled_entries.remove(entry);
			disabled_entries.insert(entry);
		}
	}
}

bool logger::enabled(const QString& entry) const
{
	QString e = entry.left(entry.indexOf("#"));
#ifdef NDEBUG
	return
#else
	bool ok =
#endif
	(enabled_entries.contains(e) || enabled_entries.empty()) && (!disabled_entries.contains(e));
#ifndef NDEBUG
	if (!ok)
	{
		m_skipped.insert(e);
	}

	return ok;
#endif
}

void logger::write(warn_type warning_type, const QString& entry, const QString& text) const
{
	if (!text.isEmpty() && enabled(entry))
	{
		QMutexLocker locker(&mutex);

		QFileInfo filepath = QDateTime::currentDateTime().toString(m_filename_format);
		QDir().mkpath(filepath.path());

		QString file_text(QString("%1: 0x%2: ")
			.arg(QDateTime::currentDateTime().toString(m_prefix_format))
			.arg((unsigned int)QThread::currentThreadId(), 2 * sizeof(unsigned int), 16, QChar('0'))
			);

		switch (warning_type){
			case t_info:
				file_text += "INFO";
				break;
			case t_warning:
				file_text += "WARNING";
				break;
			case t_fatal:
				file_text += "FATAL";
				break;
			case t_debug:
				file_text += "DEBUG";
				break;
			default:
				file_text += "UNKNOWN";
				break;
		}

		QString f_prefix (file_text+": "+entry+": ");
		QString s_prefix(f_prefix.length(), ' ');

		QStringList vt_text(text.split("\n", QString::SkipEmptyParts));
		
		QString f_write(f_prefix+vt_text.takeFirst()+"\r\n");

		QString tt;
		Q_FOREACH(tt, vt_text)
			f_write += s_prefix + tt + "\r\n";

		QFile ofile(filepath.filePath());
		if(ofile.open(QIODevice::WriteOnly | QIODevice::Append))
		{
			QTextStream mystream(&ofile);
			mystream.setCodec("UTF-8");

			if(!ofile.size())
				mystream << bom;

			mystream << f_write << flush;
			ofile.close();
		}

#ifndef NDEBUG
		std::wcout << f_write.toStdWString() << std::flush;
#if defined(_WIN32)
		OutputDebugStringW(f_write.toStdWString().c_str());
#endif
#endif
	}
}


void logger::info(const QString& entry, const QString& text) const
{
	write(t_info,entry,text);	
}
void logger::warning(const QString& entry, const QString& text) const
{
	write(t_warning,entry,text);
}
void logger::debug(const QString& entry, const QString& text) const
{
	write(t_debug,entry,text);
}
void logger::fatal(const QString& entry, const QString& text) const
{
	write(t_fatal,entry,text);
}
