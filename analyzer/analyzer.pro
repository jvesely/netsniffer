######################################################################
# Automatically generated by qmake (2.01a) Thu Feb 28 00:55:41 2008
######################################################################

TEMPLATE = app
TARGET = analyzer


DEPENDPATH *= .
INCLUDEPATH *= . ../headers src
CONFIG *= qt debug precompile_header qt_warn
QT *= network gui
RESOURCES *=	res/images.qrc
RC_FILE *= res/analyzer.rc
VPATH += src src/gui forms ../headers
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = forms
RCC_DIR = res
# Input

PRECOMPILED_HEADER = AnalyzerInc.h

HEADERS +=	Analyzer.h \
						AnalyzeDialog.h \
						ARecognizerEngine.h \
						Connection.h \
						ConnectionModel.h \
						Control.h \
						debug.h \
						IAnalyzer.h \
						IConnection.h \
						IDevice.h \
						IDeviceList.h \
						IPlugin.h \
						IRecognizer.h \
						MainWindow.h \
						Packet.h \
						OptionsDialog.h \
						PacketSorter.h \
						PluginCenter.h \
						PluginLoader.h \
						Recognizer.h \
						RManager.h \
						SorterPool.h \
						SafeHash.h \
						SafeQueue.h \
						Singleton.h \
						errors.h \
						protocol.h \
						Updater.h
SOURCES += 	Analyzer.cpp \
						AnalyzeDialog.cpp \
						Control.cpp \
						Packet.cpp \
						Connection.cpp \
						ConnectionModel.cpp \
						main.cpp \
						MainWindow.cpp \
						OptionsDialog.cpp \
						PacketSorter.cpp \
						PluginCenter.cpp \
						PluginLoader.cpp \
						RManager.cpp \
						Recognizer.cpp \
						SorterPool.cpp
FORMS		+=	MainWindow.ui \
						AnalyzeDialog.ui \
						OptionsDialog.ui \
						PluginCenter.ui \
						RecognizerLoad.ui
