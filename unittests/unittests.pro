include(../coverage.pri)

CONFIG += testcase
QMAKE_CXXFLAGS += -std=c++11
DEFINES += QT_NO_KEYWORDS
TARGET = tst_mediaplayerplugin

QT += multimedia opengl quick testlib

INCLUDEPATH += ../src/aal \
    /usr/include/qt5/QtMultimedia \
    /usr/include/hybris \
    /usr/include/libqtubuntu-media-signals \
    /usr/include/mediascanner-2.0
LIBS += \
    -lqtubuntu-media-signals \
    -lmediascanner-2.0

HEADERS += \
    ../src/aal/aalmediaplayercontrol.h \
    ../src/aal/aalmediaplayerservice.h \
    ../src/aal/aalmediaplayerserviceplugin.h \
    ../src/aal/aalvideorenderercontrol.h \
    ../src/aal/aalmetadatareadercontrol.h \
    tst_mediaplayerplugin.h \
    tst_metadatareadercontrol.h \
    player.h \
    service.h

SOURCES += \
    tst_mediaplayerplugin.cpp \
    tst_metadatareadercontrol.cpp \
    player.cpp \
    service.cpp \
    ../src/aal/aalmediaplayercontrol.cpp \
    ../src/aal/aalmetadatareadercontrol.cpp \
    ../src/aal/aalmediaplayerservice.cpp \
    ../src/aal/aalmediaplayerserviceplugin.cpp \
    ../src/aal/aalvideorenderercontrol.cpp

videos.path = /home/phablet/Videos
music.path = /home/phablet/Music
pictures.path = /home/phablet/Pictures

videos.files = videos/*
music.files = audio/*
pictures.files = images/*

# Prevent qmake from trying to strip everything
QMAKE_STRIP = echo

unix {
  INSTALLS += videos music pictures
  QMAKE_INSTALL_FILE = install -m 644 -p -o phablet -g phablet
}
