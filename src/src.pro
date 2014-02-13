include(../coverage.pri)
TARGET = aalmediaplayer
QT += multimedia opengl
TEMPLATE = lib
CONFIG += plugin
QMAKE_CXXFLAGS += -std=c++11
DEFINES += QT_NO_KEYWORDS
PLUGIN_TYPE = mediaservice

target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris /usr/include/libqtubuntu-media-signals
LIBS += \
    -lhybris-common -lubuntu_application_api -lmedia -lqtubuntu-media-signals \
    -L/usr/local/lib/arm-linux-gnueabihf -lmedia-hub-client

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
