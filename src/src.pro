include(../coverage.pri)
TARGET = aalmediaplayer
#QT += multimedia-private opengl
QT += multimedia opengl

PLUGIN_TYPE = mediaservice

load(qt_plugin)
target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

#INCLUDEPATH += /usr/include/hybris/mediaplayer
INCLUDEPATH += /home/jhodapp/google/ics/media_wrapper/compat/media
LIBS += -L/usr/lib/arm-linux-gnueabi -L/home/jhodapp/google/ics/media_wrapper/hybris -lmediaplayer

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
