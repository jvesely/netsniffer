######################################################################
# Automatically generated by qmake (2.01a) Thu Feb 28 00:55:41 2008
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH *= .
INCLUDEPATH *= .;../headers
CONFIG *= qt
QT *= network gui
RESOURCES *=	res/images.qrc


# Input
HEADERS += 	CAnalyzer.h \
						IPHeader.h \
						CPacket.h \
						mainWindow.h \
						TCPHeader.h \
						UDPHeader.h \
						CConnection.h \
						CConnectionModel.h \
						../headers/IDevice.h \
						../headers/IDevList.h
SOURCES += 	CAnalyzer.cpp \
						IPHeader.cpp \
						CPacket.cpp \
						UDPHeader.cpp \
						TCPHeader.cpp \
						CConnection.cpp \
						CConnectionModel.cpp \
						main.cpp \
						mainWindow.cpp
FORMS		+=	mainWindow.ui		
#LIBS += -lpcap
