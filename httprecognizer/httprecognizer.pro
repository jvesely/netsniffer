TEMPLATE = lib 
QT = core gui network webkit
CONFIG += precompile_header warn_on debug_and_release

DEPENDPATH += .
INCLUDEPATH += . ../headers ../shared src
VPATH += forms src ../headers ../shared

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

# Input

PRECOMPILED_HEADER = HttpRecognizerInc.h

HEADERS += \
	Http.h \
	HttpConnection.h \
	HttpConnectionData.h \
	HttpGlobalCache.h \
	HttpRecognizer.h \
	HttpRecognizerPlugin.h \
	IAnalyzer.h \
	IConnection.h \
	IPlugin.h \
	IRecognizer.h \
	gui/CacheAccessManager.h \
	gui/HttpPresenter.h \
	gui/HttpConnectionModel.h \
	gui/HttpNetworkReply.h \

SOURCES += \
	HttpConnectionData.cpp \
	HttpRecognizer.cpp \
	HttpRecognizerPlugin.cpp \
	gui/CacheAccessManager.cpp \
	gui/HttpPresenter.cpp \
	gui/HttpConnectionModel.cpp \

FORMS += \
	HttpPresenter.ui
