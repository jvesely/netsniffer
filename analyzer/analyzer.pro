######################################################################
# Automatically generated by qmake (2.01a) Thu Feb 28 00:55:41 2008
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .;../headers
CONFIG += qt
QT += network

# Input
HEADERS += 	cAnalyzer.h \
						IPheader.h \
						CPacket.h \
						mainWindow.h \
						TCPHeader.h \
						UDPHeader.h \
						../headers/IDevice.h \
						../headers/IDevList.h
SOURCES += 	cAnalyzer.cpp \
						IPheader.cpp \
						CPacket.cpp \
						UDPHeader.cpp \
						main.cpp \
						mainWindow.cpp
#LIBS += -lpcap
