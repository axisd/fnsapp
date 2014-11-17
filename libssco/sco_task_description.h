#ifndef _SCO_TASK_DESCRIPTION
#define _SCO_TASK_DESCRIPTION

#include <QString>
#include <QMap>

#include <boost/shared_ptr.hpp>

enum SCOTaskStatus
{
	SCO_TASK_STARTED,
	SCO_TASK_IN_PROGRESS,
	SCO_TASK_FINISHED
};

enum SCOTaskTypes
{
	SCO_IMPORT_DATA,
	SCO_EXPORT_RECEIPT,
	SCO_EXPORT_ALL,
	SCO_REGISTER_TERMINAL,
	SCO_GENERATE_DISCOUNTS,
	SCO_REMOVE_TASK
};

struct SCOTaskDescription
{
	unsigned int m_owner;
	unsigned int m_efts;
	unsigned int m_status;
	QString m_result;

	SCOTaskDescription() {}
	SCOTaskDescription(unsigned int __owner, unsigned int __efts): m_owner(__owner), m_efts(__efts), m_status(SCO_TASK_STARTED) {};
};
typedef QMap<QString, SCOTaskDescription > sco_tasks;

#endif
