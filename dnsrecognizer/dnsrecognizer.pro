TEMPLATE = lib 
QT = core gui network
CONFIG += precompile_header warn_on debug_and_release

DEPENDPATH += .
INCLUDEPATH += . ../headers src ../shared
VPATH += src ../headers ../shared

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

PRECOMPILED_HEADER = DnsRecognizerInc.h

HEADERS += \
	Dns.h \
	DnsRecognizer.h \
	DnsRecognizerPlugin.h \
	errors.h \
	IAnalyzer.h \
	IConnection.h \
	IDNSCache.h \
	IPlugin.h \
	IRecognizer.h \

SOURCES += \
	DnsRecognizer.cpp \
	DnsRecognizerPlugin.cpp \
