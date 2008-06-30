#include <QtGui>
#include <QFileDialog>
#include "mainWindow.h"

#define PATH "./libNetDump.so"


mainWindow::mainWindow(){
	
	setupUi(this);

	NICs = new QComboBox();

	toolBar->addWidget(NICs);
	
	analyzer = new CAnalyzer (this);
	listView->setModel(analyzer->store);
	connect(NICs, SIGNAL(currentIndexChanged(int)), analyzer, SLOT(selectNIC(int)));
	
	//connect(analyzer, SIGNAL(started()), this, SLOT(started()) );
	//connect(analyzer, SIGNAL(stopped()), this, SLOT(stopped()) );
	connect(actionStart, SIGNAL(triggered()), analyzer, SLOT(startNIC()));
	connect(actionStop, SIGNAL(triggered()), analyzer, SLOT(stopNIC()));
	connect(analyzer, SIGNAL(devsChanged(QStringList)), this, SLOT(setSelector(QStringList)));

	loadSniffer();

	readSettings();
}
/*----------------------------------------------------------------------------*/
void mainWindow::loadSniffer(QString path) {
	if (path.isNull())
		path = QFileDialog::getOpenFileName(this,
		     tr("Load Plugin"), ".", tr("Plugins (*.so *.dll)"));
	if (analyzer)
		analyzer->loadSniffer(path);
}
/*----------------------------------------------------------------------------*/
void mainWindow::setSelector(QStringList devs) {
	NICs->clear();
	NICs->addItems(devs);
	NICs->setCurrentIndex(0); // select first
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
