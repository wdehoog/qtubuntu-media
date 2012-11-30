include(../coverage.pri)

CONFIG += testcase
TARGET = tst_mediaplayerplugin

QT += testlib
#multimedia-private opengl

SOURCES += tst_mediaplayerplugin.cpp #\
#../src/aalmediaplayerservice.cpp \
#../src/aalmediaplayercontrol.cpp \
#../src/aalvideorenderercontrol.cpp

INCLUDEPATH += ../src \
    /opt/qt5/include/QtMultimedia \
    /usr/include/hybris
#LIBS += -lhybris_ics -lubuntu_application_api -lmediaplayer
