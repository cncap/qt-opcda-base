#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T00:17:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = D-Opc
TEMPLATE = app


SOURCES += main.cpp\
    clientgui.cpp \
    include/opcda/OPCClient.cpp \
    include/opcda/opccomn_i.c \
    include/opcda/Opcda_i.c \
    include/opcda/OpcEnum_i.c \
    include/opcda/copcdatacallback.cpp \
    include/opcda/copctransaction.cpp

HEADERS  += clientgui.h \
    include/opcda/wldef.h \
    include/opcda/OPCClient.h \
    include/opcda/opccomn.h \
    include/opcda/Opcda.h \
    include/opcda/OpcEnum.h \
    include/opcda/opcerror.h \
    include/opcda/copcdatacallback.h \
    include/opcda/copctransaction.h

FORMS    += clientgui.ui
LIBS += -luuid -loleaut32 -lole32 -lcomctl32  -lWs2_32
