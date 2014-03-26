include(../coverage.pri)

CONFIG += testcase
QMAKE_CXXFLAGS += -std=c++11
DEFINES += QT_NO_KEYWORDS
TARGET = tst_mediaplayerplugin

QT += multimedia opengl quick

INCLUDEPATH += ../src \
    /usr/include/qt5/QtMultimedia \
    /usr/include/hybris \
    /usr/include/libqtubuntu-media-signals
LIBS += \
    -lqtubuntu-media-signals \
    -lmedia-hub-client

HEADERS += \
    ../src/aalmediaplayercontrol.h \
    ../src/aalmediaplayerservice.h \
    ../src/aalmediaplayerserviceplugin.h \
    ../src/aalvideorenderercontrol.h

SOURCES += \
    tst_mediaplayerplugin.cpp \
    player.cpp \
    ../src/aalmediaplayercontrol.cpp \
    ../src/aalmediaplayerservice.cpp \
    ../src/aalmediaplayerserviceplugin.cpp \
    ../src/aalvideorenderercontrol.cpp
