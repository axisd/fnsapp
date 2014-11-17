#ifndef _FNS_TASK_DESCRIPTION_V1_H
#define _FNS_TASK_DESCRIPTION_V1_H

#include <QString>
#include <QMap>

#include <boost/shared_ptr.hpp>

enum FNSTaskStatus
{
    FNS_TASK_STARTED,
    FNS_TASK_IN_PROGRESS,
    FNS_TASK_FINISHED_FAIL,
    FNS_TASK_FINISHED_SUCCESS
};

enum FNSTaskTypes
{
    FNS_SEND_RECEIPT,
    FNS_SEND_MONEY_OPERATION,
    FNS_SEND_Z_REPORT,
    FNS_SEND_X_REPORT
};

struct FNSTaskDescription
{
	unsigned int m_owner;
	unsigned int m_efts;
	unsigned int m_status;
	QString m_result;

    FNSTaskDescription() {}
    FNSTaskDescription(unsigned int __owner, unsigned int __efts): m_owner(__owner), m_efts(__efts), m_status(FNS_TASK_STARTED) {};
};

typedef QMap<QString, FNSTaskDescription > sco_tasks;

#endif
