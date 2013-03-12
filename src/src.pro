include(../coverage.pri)
TARGET = aalmediaplayer
QT += multimedia-private opengl
TEMPLATE = lib
CONFIG += plugin
PLUGIN_TYPE = mediaservice

target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris /usr/include/libqtubuntu-media-signals
LIBS += -lhybris_ics -lubuntu_application_api -lmediaplayer -lqtubuntu-media-signals

OTHER_FILES += aalmediaplayer.json

HEADERS += \
    aalmediaplayercontrol.h \
    aalmediaplayerservice.h \
    aalmediaplayerserviceplugin.h \
    aalvideorenderercontrol.h

SOURCES += \
    aalmediaplayercontrol.cpp \
    aalmediaplayerservice.cpp \
    aalmediaplayerserviceplugin.cpp \
    aalvideorenderercontrol.cpp
