######################################################################
# Automatically generated by qmake (2.01a) Sat 17. Nov 15:42:45 2007
######################################################################

TEMPLATE = lib
TARGET = 
DEPENDPATH += .;../../libs
INCLUDEPATH += .;./include;../anlyzer
CONFIG		+= qt warn_on embed_manifest_exe

win32:debug:CONFIG += console 


# Input
HEADERS += \
	IDevice.h \
	IDevList.h \
	PcapDev.h \
	PcapList.h
SOURCES += \
	PcapDev.cpp \
	PcapList.cpp

win32:LIBS += ./libs/wpcap.lib