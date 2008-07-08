#include <QtGui>
#include <QFileDialog>
#include "MainWindow.h"

#define PATH "./libNetDump.so"


MainWindow::MainWindow(){
	
	setupUi(this);

	NICs = new QComboBox(toolBar);

	QToolButton * purgeButton = (QToolButton*)toolBar->widgetForAction(actionPurge);
	deathMenu = new QMenu(purgeButton);

	purgeButton->setMenu(deathMenu);
	purgeButton->setPopupMode(QToolButton::MenuButtonPopup);
	
	deathMenu->addAction(actionAuto_Purge);

	toolBar->addWidget(NICs);
	//toolBar->addSeparator();
	//toolBar->addWidget(deathWarden);
	store = new ConnectionModel();
	listView->setModel(store);
	
	connect(NICs, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectNIC(int)));
	connect(actionStart, SIGNAL(triggered()), this, SIGNAL(startNIC()));
	connect(actionStop, SIGNAL(triggered()), this, SIGNAL(stopNIC()));
	connect(actionLoad_Sniffer, SIGNAL(triggered()), this, SIGNAL(newSniffer()));
	connect(actionPurge, SIGNAL(triggered()), this, SIGNAL(purge()));
	connect(actionAuto_Purge, SIGNAL(triggered(bool)), this, SIGNAL(autoPurge(bool)));
	// forward signals

	readSettings();
}
/*----------------------------------------------------------------------------*/
void MainWindow::display(Connection* con, bool fresh){
	  if (fresh)
	    store->insertConnection(con);
	  else
	    store->changeConnection(con);
}
/*----------------------------------------------------------------------------*/
QString MainWindow::getPlugin(QString path) {
	if (path.isNull())
		path = QFileDialog::getOpenFileName(this,
		     tr("Load Plugin"), ".", tr("Plugins (*.so *.dll)"));
	return(path);
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
void MainWindow::started(QString devname) {
	actionStart->setEnabled(false);
	actionStop->setEnabled(true);
	setWindowTitle(windowTitle().append(" listening on " + devname ));
}
/*----------------------------------------------------------------------------*/
void MainWindow::stopped(QString devname) {
	Q_UNUSED(devname);
	actionStart->setEnabled(true);
	actionStop->setEnabled(false);
	setWindowTitle("IPAnalyzer");
}
