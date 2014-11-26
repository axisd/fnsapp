#ifndef GNIVC_H
#define GNIVC_H

#include "libvpm/libvpm-c.h"
#include "libvpm/instance.h"
#include "libvpm/settings.h"
#include "libvpm/settings_loader.h"
#include "libvpm/libvpm-cpp.h"

// Qt
#include <QString>
#include <QObject>

// Main
#include "libssco/receipt_v1.h"
#include "libssco/shift_close_v1.h"
#include "libssco/money_operation_v1.h"


class CGnivcSender : public QObject
{
    Q_OBJECT
public:
    CGnivcSender(QObject *parent = 0);
    ~CGnivcSender();

	bool initLibrary(const QString& __iniFile);    

    void SendReceipt(const SSCO::ReceiptV1Ptr __receipt, QString& __fiscalText);
    void SendZReport(const SSCO::ShiftCloseV1Ptr __shiftNumber);
	void SendXReport();
    void SendMoneyOperation(const SSCO::MoneyOperationV1Ptr __moHeader);
	void StopInstance(void);

private:
    libvpm::Instance* m_instance;
	void ClearToken(const QString& __fileName);
    void procEvent(const int pause);
};


#endif
