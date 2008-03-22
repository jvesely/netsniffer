#include <QtGui>

#include <QPluginLoader>
#include "mainWindow.h"

#define PATH "./libNetDump.so"


mainWindow::mainWindow(){
	setWindowTitle("NetSniffer");
	
	setupUi(this);

	NICs = new QComboBox();
	toolBar->addWidget(NICs);
	
	IDevList * devlist = qobject_cast<IDevList*>(loadPlugin(PATH));

	if (devlist)
		NICs->insertItems(0, devlist->getList());
	
	analyzer = new cAnalyzer ((IDevList*)devlist);
	connect(NICs, SIGNAL(currentIndexChanged(int)), analyzer, SLOT(selectNIC(int)));
	connect(analyzer, SIGNAL(analyzed(QString)), this, SLOT(print(QString)) );
	connect(analyzer, SIGNAL(captureStarted()), this, SLOT(started()) );
	connect(analyzer, SIGNAL(captureStopped()), this, SLOT(stopped()) );
	connect(actionStart, SIGNAL(triggered()), this, SLOT(start()));
	connect(actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	
	int dev = 2;

	qDebug() <<"Selecting device " << dev <<": "<< (bool)analyzer->selectNIC(dev);

	readSettings();
}
/*----------------------------------------------------------------------------*/
void mainWindow::readSettings(){
	QSettings settings("Student", "NetSniffer");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(640, 480)).toSize();
	resize(size);
	move(pos);
}
/*----------------------------------------------------------------------------*/
void mainWindow::writeSettings(){
	QSettings settings("Student", "NetSniffer");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}
/*----------------------------------------------------------------------------*/
QObject * mainWindow::loadPlugin(QString path){
	qDebug() << "PATH: " << path;
	QPluginLoader loader(path);
	qDebug() << "Loading plugin ... " << loader.errorString();
	qDebug() << loader.load() << loader.errorString() << endl;
	return loader.instance();
}
/*----------------------------------------------------------------------------*/
void mainWindow::print(QString text){
	listWidget->addItem(text);
}
/*----------------------------------------------------------------------------*/
void mainWindow::start(){
	if (analyzer)
		analyzer->startNIC();
}
/*----------------------------------------------------------------------------*/
void mainWindow::stop(){
	if (analyzer)
		analyzer->stopNIC();
}
/*----------------------------------------------------------------------------*/
void mainWindow::started(){
	actionStart->setEnabled(false);
	actionStop->setEnabled(true);
	print( analyzer->getDev()->getName().prepend("Capturing on ").append(" started"));
}
/*----------------------------------------------------------------------------*/
void mainWindow::stopped(){
	actionStart->setEnabled(true);
	actionStop->setEnabled(false);
	print( analyzer->getDev()->getName().prepend("Capturing on ").append(" stopped"));
}
