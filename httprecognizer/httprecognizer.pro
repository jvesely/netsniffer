TEMPLATE = lib 
QT = core gui network webkit
CONFIG += precompile_header warn_on debug_and_release

DEPENDPATH += .
INCLUDEPATH += . ../headers ../shared src
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

PRECOMPILED_HEADER = HttpRecognizerInc.h

HEADERS += \
	HttpRecognizer.h \
	HttpRecognizerPlugin.h \
	IAnalyzer.h \
	IConnection.h \
	IPlugin.h \
	IRecognizer.h \

SOURCES += \
	HttpRecognizer.cpp \
	HttpRecognizerPlugin.cpp \
