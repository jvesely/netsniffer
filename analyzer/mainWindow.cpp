#include <QtGUI>
#include <QPluginLoader>
#include "mainWindow.h"

mainWindow::mainWindow(){
	setWindowTitle("Ignitor");

	view = new QListWidget(this);
	setCentralWidget(view);

	createActions();
	createMenus();

	stopCapture->setEnabled(false);

	QObject * devlist = loadPlugin("d:\\progs\\C\\netsniffer\\NetDump\\debug\\NetDump.dll");
	
	analyzer = new cAnalyzer (qobject_cast<IDevList *>(devlist));
	connect(analyzer, SIGNAL(analyzed(QString)), this, SLOT(print(QString)) );
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
  connect(startCapture, SIGNAL(triggered()), this, SLOT(start()));

	stopCapture = new QAction(tr("Stop &capture"), this);
  stopCapture->setShortcut(tr("Ctrl+c"));
  stopCapture->setStatusTip(tr("Stop capture on selected interface"));
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
	stopCapture->setEnabled(true);
	startCapture->setEnabled(false);
	print("Capturing on ... started");
	analyzer->startNIC();
}
/*-------------------------------------------------------------------------------*/
void mainWindow::stop(){
	analyzer->stopNIC();
	startCapture->setEnabled(true);
	stopCapture->setEnabled(false);
	print("Capturing on ... stopped");
}
