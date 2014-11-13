#if !defined(LOGGER_H)
#define LOGGER_H

// Qt
#include <QString>
#include <QSet>

/// @todo только метод write является потокозащищённым
class logger
{
public:
	typedef QSet<QString> string_list_t;
#ifndef NDEBUG
	static string_list_t m_skipped;
#endif

private:
	QString m_filename_format;
	QString m_prefix_format;
	string_list_t enabled_entries;
	string_list_t disabled_entries;

public:
	logger(const QString& __filename_format, const QString& __prefix_format);

	void set_format(const QString& __filename_format, const QString& __prefix_format = "yyyy/MM/dd hh:mm:ss");

	/// @todo решить вопрос с регистронезависимым поиском
	bool enabled(const QString& entry) const;
	enum warn_type{t_info,t_warning,t_debug,t_fatal};
	void fill_entries(const QString& Param);
	void add_entries(const QString& Param);
	void remove_entries(const QString& Param);

	void write(warn_type warning_type, const QString& entry, const QString& text) const;
	void info(const QString& entry, const QString& text) const;
	void warning(const QString& entry, const QString& text) const;
	void debug(const QString& entry, const QString& text) const;
	void fatal(const QString& entry, const QString& text) const;
};

class logger_wrapper
{
	const logger& m_log;
	QString m_entry;
	const bool m_enabled;

public:
	logger_wrapper(const logger& __log, const QString& __entry);
	
	bool enabled() const;
	void info(const QString& text);
	void warning(const QString& text);
	void debug(const QString& text);
	void fatal(const QString& text);
};

#endif
