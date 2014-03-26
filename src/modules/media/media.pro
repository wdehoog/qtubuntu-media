TARGET = ubuntumediaplugin
TEMPLATE = lib
CONFIG += plugin 

QT += qml quick quick-private core-private qml-private multimedia multimedia-private qtmultimediaquicktools-private

#QMAKE_CXXFLAGS += -std=c++11 -fvisibility=hidden -fvisibility-inlines-hidden
#QMAKE_CXXFLAGS_RELEASE += -Werror     # so no stop on warning in debug builds
#QMAKE_LFLAGS = -std=c++11 -Wl,-no-undefined
QMAKE_CXXFLAGS += -std=c++11

SOURCES = plugin.cpp \
          qubuntumedia.cpp

HEADERS = qubuntumedia.h \
          logging.h

target.files += libubuntumediaplugin.so qmldir
target.path += $$[QT_INSTALL_IMPORTS]/Ubuntu/Media
INSTALLS += target
