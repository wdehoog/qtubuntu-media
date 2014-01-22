include(../coverage.pri)

CONFIG += testcase
TARGET = tst_mediaplayerplugin

QT += testlib
QT += multimedia opengl

INCLUDEPATH += ../src \
    /usr/include/qt5/QtMultimedia \
    /usr/include/hybris \
    /usr/include/libqtubuntu-media-signals
LIBS += -lqtubuntu-media-signals

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
