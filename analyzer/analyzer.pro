######################################################################
# Automatically generated by qmake (2.01a) Sat 17. Nov 15:42:45 2007
######################################################################

TEMPLATE = app
TARGET = 
#DEPENDPATH += .;../../libs
#INCLUDEPATH += .;./include
CONFIG		+= qt warn_on embed_manifest_exe

win32:debug:CONFIG += console 


# Input
HEADERS += \
  cAnalyzer.h \
  cPacket.h \
  IDevice.h \
  IDevlist.h 
SOURCES += \
  cAnalyzer.cpp \
	main.cpp 

#win32:LIBS += ./libs/wpcap.lib