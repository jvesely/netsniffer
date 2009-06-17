######################################################################
# Automatically generated by qmake (2.01a) Thu Feb 28 00:55:41 2008
######################################################################

TEMPLATE = app
TARGET = analyzer


DEPENDPATH *= . ../header ../shared src
INCLUDEPATH *= . src ../headers ../shared
CONFIG *= qt precompile_header qt_warn
QT *= network gui
RESOURCES *=	res/images.qrc
RC_FILE *= res/analyzer.rc
VPATH += src forms ../headers ../shared
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = forms
RCC_DIR = res
# Input

PRECOMPILED_HEADER = AnalyzerInc.h

HEADERS +=	Analyzer.h \
						Connection.h \
						DNSCache.h \
						debug.h \
						errors.h \
						IAnalyzer.h \
						IConnection.h \
						IDevice.h \
						IDeviceList.h \
						IDNSCache.h \
						IOptionsTab.h \
						IPlugin.h \
						IRecognizer.h \
						Packet.h \
						PacketJob.h \
						PluginLoader.h \
						protocol.h \
						ProtocolHeaders.h \
						Singleton.h \
						TCPConnection.h \
						UDPConnection.h \
						Updater.h \
						gui/ConnectionModel.h \
						gui/Control.h \
						gui/DNSCacheModel.h \
						gui/DNSOptions.h \
						gui/DropArea.h \
						gui/MainWindow.h \
						gui/OptionsDialog.h \
						gui/uitexts.h \
						gui/PluginCenter.h \
						struct/SafeHash.h \
						struct/SafeQueue.h \

SOURCES += 	Analyzer.cpp \
						Connection.cpp \
						ConnectionJob.cpp \
						DNSCache.cpp \
						main.cpp \
						Packet.cpp \
						PacketJob.cpp \
						PluginLoader.cpp \
						TCPConnection.cpp \
						UDPConnection.cpp \
						gui/ConnectionModel.cpp \
						gui/Control.cpp \
						gui/DNSCacheModel.cpp \
						gui/DNSOptions.cpp \
						gui/DropArea.cpp \
						gui/MainWindow.cpp \
						gui/OptionsDialog.cpp \
						gui/uitexts.cpp \
						gui/PluginCenter.cpp \

FORMS		+=	MainWindow.ui \
						OptionsDialog.ui \
						PluginCenter.ui \
						RecognizerLoad.ui \
						CacheView.ui \
