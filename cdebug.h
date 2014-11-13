// Инструментарий предназначенный для упрощения отладки и
// локализации ошибок.
#if !defined(CDEBUG_H)
#define CDEBUG_H

#include "logger.h"

#include <boost/program_options.hpp>

// Исключение - ошибка в программе
class internal_error: public std::logic_error
{
public:
	internal_error(const QString& message) : std::logic_error(message.toStdString())
	{
	}
};


QString __source_point__(const QString& __file, int __line, const QString& __function);
#define __SOURCE_POINT__ __source_point__(__FILE__, __LINE__, __FUNCTION__)

// Summary
// Проверка утверждения.
//
// Parameters
// expr :  Утверждение для проверки
// Description
// Если выражение ложно, будет сформирована строка в файле
// протокола с помощью DEBUG_THROW. В качестве сообщения будет
// указано точное местонахождение в исходном файле и выражение,
// которое дало ложь при проверке.
//
// See Also
// <LINK cdebug.h_example, Пример использования cdebug.h>
#define DEBUG_ASSERT(expr) {if (!(expr)) { DEBUG_THROW(QString("expression '%1' failed").arg(#expr)); }}

// Summary
// Проверка утверждения.
//
// Parameters
// expr :  Выражение истинность которого нам необходимо проверить.
// message :  Диагностическое сообщение.
// Description
// Если выражение ложно, будет сформирована строка в файле
// протокола с помощью DEBUG_THROW. Будет
// указано точное местонахождение в исходном файле и пользовательское сообщение.
//
// See Also
// <LINK cdebug.h_example, Пример использования cdebug.h>
#define DEBUG_ASSERT_EX(expr, message) {if (!(expr)) { DEBUG_THROW(message); }}

// Summary
// Генерация исключения internal_error с записью в .log файл.
//
// Parameters
// message :  Сообщение
// Description
// Сразу за записью сообщения будет сгенерировано исключение internal_error с тем же сообщением.
#define DEBUG_THROW(message) \
	{DEBUG_CHECKPOINT(logger::t_fatal, message); throw internal_error(QString("%1").arg(message));}

// Summary
// Сообщение в файл протокола с указанием места в исходном
// тексте.
//
// Parameters
// cat :	  Категории сообщения, используется для фильтрации. В
//  		  данный момент не используется.
// message :  Сообщение.
// Description
// Фактически использует LOG_MESSAGE, но в дополнение к самому
// сообщению указывает точное местонахождение в исходном тексте.
// Например:
// <CODE>
// 15:55.03 : ukm-client.cpp:2(352):main: Hello, World!
// </CODE>
// Это означает, что в файле ukm-client.cpp (версия 2 в
// Microsoft SourceSafe) в строке 352 было выведено это
// сообщение.
// See Also
// <LINK cdebug.h_example, Пример использования cdebug.h>
#define DEBUG_CHECKPOINT(cat, message) LOG_MESSAGE((cat), QString("debug#") + __SOURCE_POINT__, (message))

// Summary
// Помещение строки сообщения в файл протокола.
//
// Parameters
// cat :	  Категории сообщения, используется для фильтрации. В
//  		  данный момент не используется.
// message :  Сообщение.
// Description
// Данный макрос позволяет поместить сообщение в файл протокола.
// Имя файла формируется в виде YYYYMMDD.log, где YYYY - текущий
// год, MM - текущий месяц и DD - текущий день. Например:
// <CODE>
// 20030620.log
// </CODE>
// То есть все сообщения за один день лежат в одном файле.
// Каждое сообщение пишется в отдельную строку, предварительно
// указав время возникновения сообщения.
#define DEBUG_MESSAGE(cat, message) LOG_MESSAGE((cat), "OBSOLETE", (message))

// Summary
// Помещение строки сообщения в файл протокола.
//
// Parameters
// cat :	  Категории сообщения, используется для фильтрации. В
//  		  данный момент не используется.
// src :	  Категории сообщения, используется для фильтрации. В
//  		  данный момент не используется.
// message :  Сообщение.
// Description
// Данный макрос позволяет поместить сообщение в файл протокола.
// Имя файла формируется в виде YYYYMMDD.log, где YYYY - текущий
// год, MM - текущий месяц и DD - текущий день. Например:
// <CODE>
// 20030620.log
// </CODE>
// То есть все сообщения за один день лежат в одном файле.
// Каждое сообщение пишется в отдельную строку, предварительно
// указав время возникновения сообщения.
#define LOG_MESSAGE(cat, src, message) c_log().write((cat), (src), QString("%1").arg(message))

logger& c_log();
void add_log_options_description(boost::program_options::options_description& __desc);
void process_log_options_description(const boost::program_options::variables_map& __vm);
#endif
