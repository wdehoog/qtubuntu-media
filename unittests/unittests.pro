include(../coverage.pri)

CONFIG += testcase
TARGET = tst_mediaplayerplugin

QT += testlib
QT += multimedia-private opengl

INCLUDEPATH += ../src \
    /opt/qt5/include/QtMultimedia \
    /usr/include/hybris
LIBS += -lhybris_ics -lubuntu_application_api -lmediaplayer

HEADERS += \
    ../src/aalmediaplayercontrol.h \
    ../src/aalmediaplayerservice.h \
    ../src/aalmediaplayerserviceplugin.h \
    ../src/aalvideorenderercontrol.h

SOURCES += \
    tst_mediaplayerplugin.cpp \
    ../src/aalmediaplayercontrol.cpp \
    ../src/aalmediaplayerservice.cpp \
    ../src/aalmediaplayerserviceplugin.cpp \
    ../src/aalvideorenderercontrol.cpp
