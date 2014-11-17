#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T16:25:44
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = fnsapp
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_MOC = $$QMAKE_MOC -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

TEMPLATE = app

SOURCES += main.cpp \
    fnsserver.cpp \
    gnivc.cpp \
    liblogger/cdebug.cpp \
    liblogger/logger.cpp \
    libssco/classif.cpp \
    libssco/item.cpp \
    libssco/large.cpp \
    libssco/model_barcode.cpp \
    libssco/model_classif_limitation.cpp \
    libssco/model_item_limitation.cpp \
    libssco/model_receipt_item.cpp \
    libssco/model_receipt_item_discount.cpp \
    libssco/model_receipt_payment_cash.cpp \
    libssco/model_receipt_payment_cashless.cpp \
    libssco/receipt.cpp \
    libssco/sax_serializer_base.cpp \
    libssco/shift_open.cpp \
    libssco/time_limitation.cpp \
    libssco/shift_close.cpp \
    libssco/money_operation.cpp \
    libssco/receipt_v1.cpp \
    libssco/model_receipt_item_v1.cpp \
    libssco/receipt_item_tax.cpp \
    libssco/fnsterminalprotocol.cpp

HEADERS += fnsserver.h \
    gnivc.h \
    liblogger/cdebug.h \
    liblogger/logger.h \
    libssco/cfixedpointround.h \
    libssco/classif.h \
    libssco/item.h \
    libssco/large.h \
    libssco/model_barcode.h \
    libssco/model_classif.h \
    libssco/model_classif_limitation.h \
    libssco/model_item.h \
    libssco/model_item_limitation.h \
    libssco/model_money_operation.h \
    libssco/model_receipt.h \
    libssco/model_receipt_item.h \
    libssco/model_receipt_item_discount.h \
    libssco/model_receipt_payment_cash.h \
    libssco/model_receipt_payment_cashless.h \
    libssco/model_shift_open.h \
    libssco/model_time_limitation.h \
    libssco/receipt.h \
    libssco/sax_serializer_base.h \
    libssco/shift_open.h \
    libssco/time_limitation.h \
    libssco/shift_close.h \
    libssco/model_shift_close.h \
    libssco/money_operation.h \
    libssco/model_receipt_v1.h \
    libssco/receipt_v1.h \
    libssco/model_receipt_item_v1.h \
    libssco/model_receipt_item_tax.h \
    libssco/receipt_item_tax.h \
    libssco/sco_task_description.h \
    libssco/fns_task_description.h \
    libssco/fnsterminalprotocol.h

win32 {
    INCLUDEPATH += \
        C:/home/ukm/suite/boost-1.53.0/include \
        C:/home/ukm/suite/libvpm-3.1.6-1.2.0/include

    LIBS += -LC:/home/ukm/suite/libvpm-3.1.6-1.2.0/lib -llibvpm
#    LIBS += -LC:/home/ukm/suite/boost-1.53.0-gcc44/lib -lboost_program_options-mgw44-mt-1_53

    CONFIG( debug, debug|release ) {
        # debug
        LIBS += -LC:/home/ukm/suite/boost-1.53.0/lib/boost -llibboost_program_options-vc100-mt-gd-1_53
    } else {
        # release
        LIBS += -LC:/home/ukm/suite/boost-1.53.0/lib/boost -llibboost_program_options-vc100-mt-1_53
    }
}

unix {
INCLUDEPATH += \
        /usr/local/storage/home/ukm/suite/gcc-4.5.2-boost-1.55-no-tls/include \
        /usr/local/storage/home/ukm/suite/libvpm/include

    LIBS += -L/usr/local/storage/home/ukm/suite/libvpm/lib -llibvpm
    LIBS += -L/usr/local/storage/home/ukm/suite/gcc-4.5.2-boost-1.55-no-tls/lib -lboost_program_options
}
