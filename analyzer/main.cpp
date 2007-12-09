#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df'\"")

#include <QVector>
#include <QDebug>
#include <QPointer>
#include <QPluginLoader>
#include <QThread>
#include <QApplication>
#include "IDevice.h"
#include "IDevList.h"
#include "cAnalyzer.h"
#include "mainWindow.h"

int main(int argc, char** argv){

	
	QApplication app(argc, argv);
	mainWindow window;
	
	window.show();


	return app.exec();

}