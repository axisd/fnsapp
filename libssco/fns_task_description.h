﻿#ifndef _FNS_TASK_DESCRIPTION_V1_H
#define _FNS_TASK_DESCRIPTION_V1_H

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

#endif