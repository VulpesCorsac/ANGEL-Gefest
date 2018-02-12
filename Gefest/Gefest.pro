#-------------------------------------------------
#
# Project created by QtCreator 2017-05-03T16:18:03
#
#-------------------------------------------------

QT       += core gui serialport testlib
CONFIG   += c++11 warn_on

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QT.testlib.CONFIG -= console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

QMAKE_CFLAGS_RELEASE += -O2
QMAKE_CFLAGS += -Wall -Wextra -Wfloat-equal -Wundef -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wshadow -Wdiv-by-zero
QMAKE_CFLAGS += -isystem $$[QT_INSTALL_HEADERS]

QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS += -Wall -Wextra -Wfloat-equal -Wundef -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wshadow -Wdiv-by-zero
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]

TARGET = Gefest
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        Gefest.cpp \
    ../ANGEL/Equipment/CommonFunctionsForSRS/SRS.cpp \
    qcustomplot.cpp

HEADERS  += Gefest.h \
    ../ANGEL/Equipment/CommonFunctionsForSRS/SRS.h \
    qcustomplot.h

FORMS    += Gefest.ui

RESOURCES += Resource.qrc

DISTFILES += Resource.rc \
    ConfigGefest.conf

win32:RC_FILE = Resource.rc
