######################################################################
# Automatically generated by qmake (2.01a) Thu Feb 28 00:55:41 2008
######################################################################

TEMPLATE = app
TARGET = analyzer


DEPENDPATH *= .
INCLUDEPATH *= . ../headers
CONFIG *= qt debug 
QT *= network gui
RESOURCES *=	res/images.qrc
VPATH += src forms ../headers
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = forms
RCC_DIR = res
# Input
HEADERS +=	Analyzer.h \
						AnalyzeDialog.h \
						ARecognizerEngine.h \
						Control.h \
						Recognizer.h \
						IPHeader.h \
						IConnection.h \
						Packet.h \
						MainWindow.h \
						OptionsDialog.h \
						TCPHeader.h \
						UDPHeader.h \
						Connection.h \
						ConnectionModel.h \
						IDevice.h \
						IDevList.h \
						RManager.h \
						errors.h \
						protocol.h
SOURCES += 	Analyzer.cpp \
						IPHeader.cpp \
						Control.cpp \
						Packet.cpp \
						UDPHeader.cpp \
						TCPHeader.cpp \
						Connection.cpp \
						ConnectionModel.cpp \
						main.cpp \
						MainWindow.cpp \
						OptionsDialog.cpp \
						RManager.cpp \
						Recognizer.cpp
FORMS		+=	MainWindow.ui \
						AnalyzeDialog.ui \
						OptionsDialog.ui \
						RecognizerLoad.ui
