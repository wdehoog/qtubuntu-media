include(../coverage.pri)

CONFIG += testcase
QMAKE_CXXFLAGS += -std=c++11
DEFINES += QT_NO_KEYWORDS
TARGET = tst_mediaplayerplugin

QT += multimedia opengl quick testlib

INCLUDEPATH += ../src/aal \
    /usr/include/qt5/QtMultimedia \
    /usr/include/hybris \
    /usr/include/libqtubuntu-media-signals
LIBS += \
    -lqtubuntu-media-signals \
    -lmedia-hub-client

HEADERS += \
    ../src/aal/aalmediaplayercontrol.h \
    ../src/aal/aalmediaplayerservice.h \
    ../src/aal/aalmediaplayerserviceplugin.h \
    ../src/aal/aalvideorenderercontrol.h

SOURCES += \
    tst_mediaplayerplugin.cpp \
    player.cpp \
    ../src/aal/aalmediaplayercontrol.cpp \
    ../src/aal/aalmediaplayerservice.cpp \
    ../src/aal/aalmediaplayerserviceplugin.cpp \
    ../src/aal/aalvideorenderercontrol.cpp
