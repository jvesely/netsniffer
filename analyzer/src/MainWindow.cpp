#include <QtGui>
#include <QFileDialog>
#include "MainWindow.h"

#define PATH "./libNetDump.so"


MainWindow::MainWindow(){
	
	setupUi(this);

	NICs = new QComboBox();

	toolBar->addWidget(NICs);
	store = new ConnectionModel();
	listView->setModel(store);
	
	connect(NICs, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectNIC(int)));
	connect(actionStart, SIGNAL(triggered()), this, SIGNAL(startNIC()));
	connect(actionStop, SIGNAL(triggered()), this, SIGNAL(stopNIC()));
	// forward signals

	readSettings();
}
/*----------------------------------------------------------------------------*/
void MainWindow::display(Connection* con){
	  if (con && con->packetCount() == 1)
	    store->insertConnection(con);
	  else
	    store->changeConnection(con);
}
QString MainWindow::getPlugin(QString path) {
	if (path.isNull())
		path = QFileDialog::getOpenFileName(this,
		     tr("Load Plugin"), ".", tr("Plugins (*.so *.dll)"));
	return(path);
	//emit newPlugin(path);
}
/*----------------------------------------------------------------------------*/
void MainWindow::setSelector(QStringList devs) {
	NICs->clear();
	NICs->addItems(devs);
	NICs->setCurrentIndex(0); // select first
}
/*----------------------------------------------------------------------------*/
void MainWindow::readSettings(){
	QSettings settings("Student", "NetSniffer");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(640, 480)).toSize();
	resize(size);
	move(pos);
}
/*----------------------------------------------------------------------------*/
void MainWindow::writeSettings(){
	QSettings settings("Student", "NetSniffer");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}
/*----------------------------------------------------------------------------*/
QObject * MainWindow::loadPlugin(QString path){
	qDebug() << "PATH: " << path;
	QPluginLoader loader(path);
	qDebug() << "Loading plugin ... " << loader.load() << loader.errorString() << endl;
	return loader.instance();
}
/*----------------------------------------------------------------------------*/
void MainWindow::started(QString devname) {
	actionStart->setEnabled(false);
	actionStop->setEnabled(true);
	//print( devname.prepend("Capturing on ").append(" started"));
}
/*----------------------------------------------------------------------------*/
void MainWindow::stopped(QString devname) {
	actionStart->setEnabled(true);
	actionStop->setEnabled(false);
	//print( devname.prepend("Capturing on ").append(" stopped"));
}
