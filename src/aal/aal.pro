include(../../coverage.pri)
TARGET = aalmediaplayer
QT += multimedia opengl multimedia-private
TEMPLATE = lib
CONFIG += plugin no_keywords
QMAKE_CXXFLAGS += -std=c++11
PLUGIN_TYPE = mediaservice

target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris /usr/include/libqtubuntu-media-signals \
               /usr/include/mediascanner-2.0
LIBS += \
    -lhybris-common -lubuntu_application_api -lmedia -lqtubuntu-media-signals \
    -lmedia-hub-client \
    -lmediascanner-2.0

OTHER_FILES += aalmediaplayer.json

HEADERS += \
    aalmediaplayercontrol.h \
    aalmediaplayerservice.h \
    aalmediaplayerserviceplugin.h \
    aalmetadatareadercontrol.h \
    aalvideorenderercontrol.h \
    aalmediaplaylistprovider.h \
    aalmediaplaylistcontrol.h

SOURCES += \
    aalmediaplayercontrol.cpp \
    aalmetadatareadercontrol.cpp \
    aalmediaplayerservice.cpp \
    aalmediaplayerserviceplugin.cpp \
    aalvideorenderercontrol.cpp \
    aalmediaplaylistprovider.cpp \
    aalmediaplaylistcontrol.cpp 
