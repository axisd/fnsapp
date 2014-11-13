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

TEMPLATE = app


SOURCES += main.cpp \
    fnsserver.cpp \
    model_receipt_item.cpp \
    model_receipt_item_discount.cpp \
    model_receipt_payment_cash.cpp \
    model_receipt_payment_cashless.cpp \
    receipt.cpp \
    sax_serializer_base.cpp \
    large.cpp \
    item.cpp \
    model_item_limitation.cpp \
    shift_open.cpp \
    classif.cpp \
    model_barcode.cpp \
    time_limitation.cpp \
    model_classif_limitation.cpp \
    cdebug.cpp \
    logger.cpp

HEADERS += \
    fnsserver.h \
    model_money_operation.h \
    model_receipt.h \
    model_receipt_item.h \
    model_receipt_item_discount.h \
    model_receipt_payment_cash.h \
    model_receipt_payment_cashless.h \
    receipt.h \
    sax_serializer_base.h \
    cfixedpointround.h \
    large.h \
    item.h \
    model_item.h \
    model_item_limitation.h \
    shift_open.h \
    classif.h \
    model_barcode.h \
    time_limitation.h \
    model_classif.h \
    model_classif_limitation.h \
    model_time_limitation.h \
    model_shift_open.h \
    cdebug.h \
    logger.h


win32 {
    INCLUDEPATH += C:\home\ukm\suite\boost-1.53.0-gcc44\include
    LIBS += -LC:\home\ukm\suite\boost-1.53.0-gcc44\lib -lboost_program_options-mgw44-mt-1_53
}

unix {
}
