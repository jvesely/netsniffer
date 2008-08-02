#include <QtGui>
#include <QFileDialog>
#include "MainWindow.h"
#include "AnalyzeDialog.h"
#include "OptionsDialog.h"

#define PATH "./libNetDump.so"


MainWindow::MainWindow(IAnalyzer * controlledAnalyzer){
	
	setupUi(this);

	NICs = new QComboBox(toolBar);
//
	//QToolButton * purgeButton = (QToolButton*)toolBar->widgetForAction(actionPurge);
	//deathMenu = NULL;
	//deathMenu = new QMenu(purgeButton);

	//purgeButton->setMenu(deathMenu);
	//purgeButton->setPopupMode(QToolButton::MenuButtonPopup);
	
	//deathMenu->addAction(actionAuto_Purge);

	toolBar->addWidget(NICs);
	//toolBar->addSeparator();
	//toolBar->addWidget(deathWarden);
	//attach(controlledAnalyzer);
	
	connect(actionLoad_Sniffer, SIGNAL(triggered()), this, SLOT(snifferPlugin()));
	connect(actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()));
	connect(actionAnalyze, SIGNAL(triggered()), this, SLOT(analyze()));
	connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(analyze(QModelIndex)));
	attach(controlledAnalyzer);
	readSettings();
	show();
}
/*----------------------------------------------------------------------------*/
bool MainWindow::attach(IAnalyzer * analyzer) {
	if ( !analyzer )
		return false;
	qDebug() << "Attaching analyzer..";
	view->setModel(analyzer->model());
	//setDevices(analyzer->devices());
	qDebug() << analyzer << analyzer->devices();
	qDebug() << "Connecting stuff..";
	connect(NICs, SIGNAL(currentIndexChanged(int)), analyzer, SLOT(selectDevice(int)));
	connect(actionAuto_Purge, SIGNAL(triggered(bool)), analyzer, SLOT(setAutoPurge(bool)));
	connect(this, SIGNAL(newSniffer(QString)), analyzer, SLOT(loadSnifferPlugin(QString)));
	connect(analyzer, SIGNAL(deviceChanged(IDevice *)), this, SLOT(connectDevice(IDevice *)));
	connect(analyzer, SIGNAL(devicesChanged(QStringList)), this, SLOT(setDevices(QStringList)));
	connect(analyzer, SIGNAL(error(QString)), this, SLOT(printError(QString)));
	connectDevice(analyzer->currentDevice());
	return true;
	
}
bool MainWindow::connectDevice(IDevice * device) {
	if ( !device )
		return false;
	qDebug() << "Connecting device...";
	return 
		connect(device, SIGNAL(captureStarted(QString)), this, SLOT(started(QString))) &&
		connect(actionStart, SIGNAL(triggered()), device, SLOT(captureStart())) &&
		connect(device, SIGNAL(captureStopped(QString)), this, SLOT(stopped(QString))) &&
		connect(actionStop, SIGNAL(triggered()), device, SLOT(captureStop()));
}
/*----------------------------------------------------------------------------*/
void MainWindow::snifferPlugin() {
	QString path = QFileDialog::getOpenFileName(this,
		     tr("Load Plugin"), ".", tr("Plugins (*.so *.dll)"));
	if ( !path.isEmpty())
		emit newSniffer(path);
}
/*----------------------------------------------------------------------------*/
void MainWindow::setDevices(const QStringList devs) {
	NICs->clear();
	NICs->addItems(devs);
	if ( !devs.isEmpty() ) // there is something to select from
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
/*----------------------------------------------------------------------------*/
void MainWindow::printError(QString text) {
	QMessageBox::critical(this, "Analyzer", text, QMessageBox::Ok);
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions(){

	OptionsDialog opt(this);
	IAnalyzer * mainApp = IAnalyzer::instance();
  connect(&opt, SIGNAL(newModule(QString)), mainApp, SLOT(addRecognizerPlugin(QString)));
  connect(mainApp, SIGNAL(recognizerAdded(IRecognizer *)), &opt, SLOT(addControl(IRecognizer *)));
	const QList<IRecognizer *> current = mainApp->currentRecognizers();
	qDebug() << current;
	for (QList<IRecognizer *>::ConstIterator it = current.begin(); it != current.end(); ++it) 
		opt.addControl(*it);

  opt.exec();
}

void MainWindow::analyze(QModelIndex index) {
	if (!index.isValid())
		index = view->currentIndex();
//	qDebug() << "requested index: " << index << index.isValid();
	if (!index.isValid()) // nonthing is selected
		return;
	//QWidget * interior = IAnalyzer::instance()->deepAnalyze(index);
	//qDebug() << "Got interior Widget: " << interior;
	QPointer<IConnection> con = IAnalyzer::instance()->connection(index);
	if (!con) return; // somthing wen t wrong connection does not exist
	AnalyzeDialog dialog(this, con);
	dialog.exec();
}

