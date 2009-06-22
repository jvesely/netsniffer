TEMPLATE = lib
QT = core
CONFIG += precompile_header warn_on

DEPENDPATH += .
INCLUDEPATH += . ../headers src
VPATH += src ../headers

# Output

CONFIG(debug, debug|release) {
	CONFIG_NAME = debug
} else {
	CONFIG_NAME = release
}

DESTDIR = ../bin/$${CONFIG_NAME}
OBJECTS_DIR = obj/$${CONFIG_NAME}
MOC_DIR = moc

# Input

PRECOMPILED_HEADER = SnifferInc.h 

HEADERS += \
	IDevice.h \
	IDeviceList.h \
	IPlugin.h \
	PcapDevice.h \
	PcapList.h \
	SnifferPlugin.h \

SOURCES += \
	PcapDevice.cpp \
	PcapList.cpp \
	SnifferPlugin.cpp \

win32:LIBS += wpcap.lib
!win32:LIBS += -lpcap 
