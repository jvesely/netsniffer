#include <QtGUI>
#include <QPluginLoader>
#include "mainWindow.h"
#ifndef QT_NO_DEBUG
#define PATH "..\\NetDump\\Debug\\NetDump.dll"
#else
#define PATH "..\\NetDump\\release\\NetDump.dll"
#endif //NDEBUG


mainWindow::mainWindow(){
	setWindowTitle("NetSniffer");

	view = new QListWidget(this);
	setCentralWidget(view);

	createActions();
	createMenus();

	startCapture->setEnabled(true);

	QObject * devlist = loadPlugin(PATH);
	
	
		analyzer = new cAnalyzer (qobject_cast<IDevList *>(devlist));
	

		connect(analyzer, SIGNAL(analyzed(QString)), this, SLOT(print(QString)) );
		connect(analyzer, SIGNAL(captureStarted()), this, SLOT(started()) );
		connect(analyzer, SIGNAL(captureStopped()), this, SLOT(stopped()) );
		
		qDebug() <<"Selecting device 0 ..."<< (bool)analyzer->selectNIC(0)<<endl;



	readSettings();
}
/*-------------------------------------------------------------------------------*/
void mainWindow::readSettings(){
	QSettings settings("Student", "NetSniffer");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(640, 480)).toSize();
	resize(size);
	move(pos);
}
/*-------------------------------------------------------------------------------*/
void mainWindow::writeSettings(){
	QSettings settings("Student", "NetSniffer");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}
/*-------------------------------------------------------------------------------*/
QObject * mainWindow::loadPlugin(QString path){
	QPluginLoader loader(path);
	qDebug()<<"Loading plugin ... "<< loader.load()<<endl;
	return loader.instance();
}
/*-------------------------------------------------------------------------------*/
void mainWindow::createActions(){
	
	startCapture = new QAction(tr("Start &capture"), this);
  startCapture->setShortcut(tr("Ctrl+c"));
  startCapture->setStatusTip(tr("Start capture on selected interface"));
	startCapture->setEnabled(false);
  connect(startCapture, SIGNAL(triggered()), this, SLOT(start()));

	stopCapture = new QAction(tr("Stop &capture"), this);
  stopCapture->setShortcut(tr("Ctrl+c"));
  stopCapture->setStatusTip(tr("Stop capture on selected interface"));
	stopCapture->setEnabled(false);
  connect(stopCapture, SIGNAL(triggered()), this, SLOT(stop()));
}
/*-------------------------------------------------------------------------------*/
void mainWindow::createMenus(){
	devMenu = menuBar()->addMenu(tr("&Device"));
	devMenu->addAction(startCapture);
	devMenu->addAction(stopCapture);
}
/*-------------------------------------------------------------------------------*/
void mainWindow::print(QString text){
	view->addItem(text);
}
/*-------------------------------------------------------------------------------*/
void mainWindow::start(){
	analyzer->startNIC();
}
/*-------------------------------------------------------------------------------*/
void mainWindow::stop(){
	analyzer->stopNIC();
}
/*-------------------------------------------------------------------------------*/
void mainWindow::started(){
	startCapture->setEnabled(false);
	stopCapture->setEnabled(true);
	print( analyzer->getDev()->getDesc().prepend("Capturing on ").append(" started"));
}
/*-------------------------------------------------------------------------------*/
void mainWindow::stopped(){
	startCapture->setEnabled(true);
	stopCapture->setEnabled(false);
	print( analyzer->getDev()->getDesc().prepend("Capturing on ").append(" stopped"));
}