include(../../coverage.pri)

CONFIG += testcase
QMAKE_CXXFLAGS += -std=c++11
DEFINES += QT_NO_KEYWORDS
TARGET = tst_integration

QT += multimedia opengl multimedia-private quick testlib

QT_TESTCASE_BUILDDIR = .

INCLUDEPATH += ../../src/aal \
    /usr/include/qt5/QtMultimedia \

HEADERS += \
    tst_mediaplaylist.h

SOURCES += \
    tst_mediaplaylist.cpp
