#ifndef GNIVC_H
#define GNIVC_H

#include "libvpm/libvpm-c.h"
#include "libvpm/instance.h"
#include "libvpm/settings.h"
#include "libvpm/settings_loader.h"
#include "libvpm/libvpm-cpp.h"

// Qt
#include <QString>

// Main
#include "libssco/receipt.h"
#include "libssco/shift_close.h"
#include "libssco/money_operation.h"


class CGnivcSender
{
public:
	libvpm::Instance* m_instance;

	bool initLibrary(const QString& __iniFile);
	CGnivcSender();
	~CGnivcSender();

    void SendReceipt(const SSCO::ReceiptPtr __receipt, QString& __fiscalText);
    void SendShiftClose(const SSCO::ShiftClosePtr __shiftNumber);
	void SendXReport();
    void MoneyInsert(const SSCO::MoneyOperationPtr __moHeader);
	void MoneyExtract(void);
	void StopInstance(void);

private:
	void ClearToken(const QString& __fileName);

};


#endif
