#include <QtGui>
#include <QPluginLoader>
#include "mainWindow.h"

#define PATH "./libNetDump.so"


mainWindow::mainWindow(){
	
	setupUi(this);

	NICs = new QComboBox();
	toolBar->addWidget(NICs);
	
	analyzer = new CAnalyzer (this);
	listView->setModel(&analyzer->store);
	listView->setRootIndex(analyzer->store.index(0));
	connect(NICs, SIGNAL(currentIndexChanged(int)), analyzer, SLOT(selectNIC(int)));
	
	//connect(analyzer, SIGNAL(analyzed(QString)), this, SLOT(print(QString)) );
	//connect(analyzer, SIGNAL(captureStarted()), this, SLOT(started()) );
	//connect(analyzer, SIGNAL(captureStopped()), this, SLOT(stopped()) );
	connect(actionStart, SIGNAL(triggered()), this, SLOT(start()));
	connect(actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	
	loadSniffer();

	readSettings();
}
/*----------------------------------------------------------------------------*/
void mainWindow::loadSniffer() {
	QString path(PATH);
	analyzer->setList(loadPlugin(PATH));
	NICs->insertItems(0, analyzer->getList()->getList());
	NICs->setCurrentIndex(0); //select first adapter;
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
	qDebug() << "Loading plugin ... " << loader.load() << loader.errorString() << endl;
	return loader.instance();
}
/*----------------------------------------------------------------------------*/
void mainWindow::print(QString text){
	//listWidget->addItem(text);
}
/*----------------------------------------------------------------------------*/
void mainWindow::start(){
	if (analyzer)
		analyzer->startNIC();
}
/*----------------------------------------------------------------------------*/
void mainWindow::stop() {
	if (analyzer)
		analyzer->stopNIC();
}
/*----------------------------------------------------------------------------*/
void mainWindow::started(QString devname) {
	actionStart->setEnabled(false);
	actionStop->setEnabled(true);
	if (analyzer)
		print( devname.prepend("Capturing on ").append(" started"));
}
/*----------------------------------------------------------------------------*/
void mainWindow::stopped(QString devname) {
	actionStart->setEnabled(true);
	actionStop->setEnabled(false);
	if (analyzer)
		print( devname.prepend("Capturing on ").append(" stopped"));
}
