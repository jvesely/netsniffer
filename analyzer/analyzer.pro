TEMPLATE = app
QT = core gui network
CONFIG += precompile_header warn_on
win32:debug:CONFIG += console 

DEPENDPATH += . ../header ../shared src
INCLUDEPATH += . src ../headers ../shared
VPATH += src forms ../headers ../shared

# Output

CONFIG(debug, debug|release) {
	CONFIG_NAME = debug
} else {
	CONFIG_NAME = release
}

DESTDIR = ../bin/$${CONFIG_NAME}
OBJECTS_DIR = obj/$${CONFIG_NAME}
MOC_DIR = moc
UI_DIR = forms
RCC_DIR = res

# Input
PRECOMPILED_HEADER = AnalyzerInc.h

RESOURCES += res/images.qrc
RC_FILE += res/analyzer.rc

HEADERS += \
	Analyzer.h \
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

SOURCES += 	\
	Analyzer.cpp \
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

FORMS += \
	MainWindow.ui \
	OptionsDialog.ui \
	PluginCenter.ui \
	RecognizerLoad.ui \
	CacheView.ui \
