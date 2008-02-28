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
