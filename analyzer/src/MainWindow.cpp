#include "MainWindow.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"
#include "OptionsDialog.h"
#include "uitexts.h"

#define PATH "./libNetDump.so"

#define DEBUG_TEXT "[ Main Window ]: "
#define PRINT_DEBUG qDebug() << DEBUG_TEXT

MainWindow::MainWindow()
{
	
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
	
	connect(actionLoad_Sniffer, SIGNAL(triggered()), this, SLOT(loadPlugin()));
	connect(actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()));
	connect(actionAnalyze, SIGNAL(triggered()), this, SLOT(analyze()));
	connect(actionCloseConnection, SIGNAL(triggered()), this, SLOT(closeConnection()));
	connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(analyze(QModelIndex)));
	
	attach();
	readSettings();
	
	show();
}
/*----------------------------------------------------------------------------*/
bool MainWindow::attach()
{
	PRINT_DEBUG << "Attaching analyzer..";

	view->setModel( ANALYZER->model() );
	PRINT_DEBUG << ANALYZER << ANALYZER->devices();
	PRINT_DEBUG << "Connecting stuff..";

	connect( NICs, SIGNAL(currentIndexChanged( int )), ANALYZER, SLOT(selectDevice( int )) );
	connect( actionAuto_Purge, SIGNAL(triggered( bool )), ANALYZER, SLOT(setAutoPurge( bool )) );
//	connect( this, SIGNAL(newSniffer( QString )), ANALYZER, SLOT(addPlugin( QString )) );
	connect( ANALYZER, SIGNAL(deviceChanged( IDevice * )), this, SLOT(connectDevice( IDevice * )) );
	connect( ANALYZER, SIGNAL(devicesChanged( QStringList )), this, SLOT(setDevices( QStringList )) );
	connect( ANALYZER, SIGNAL(error( QString )), this, SLOT(printError( QString )) );

	connectDevice( ANALYZER->currentDevice() );
	
	return true;
}
/*----------------------------------------------------------------------------*/
bool MainWindow::connectDevice( IDevice* device )
{
	if ( !device )
		return false;
	PRINT_DEBUG << "Connecting device...";

	return 
	   connect( device, SIGNAL(captureStarted( IDevice* )), this, SLOT(started( IDevice* )) )
	&& connect( device, SIGNAL(captureStopped( IDevice* )), this, SLOT(stopped( IDevice* )) )
	&& connect( actionStart, SIGNAL(triggered()), device, SLOT(captureStart()) )
	&& connect( actionStop, SIGNAL(triggered()), device, SLOT(captureStop()) );
}
/*----------------------------------------------------------------------------*/
void MainWindow::loadPlugin()
{
	const QString path = QFileDialog::getOpenFileName(
			this, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ) );
	ANALYZER->addPlugin( path );
}
/*----------------------------------------------------------------------------*/
void MainWindow::setDevices(const QStringList devs) {
	NICs->clear();
	NICs->addItems( devs );

	if ( !devs.isEmpty() ) {// there is something to select from
		NICs->setCurrentIndex( 0 ); // select first
	}
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
	QSettings settings( "Student", "NetSniffer" );
	
	settings.setValue( "pos", pos() );
	settings.setValue( "size", size() );
}
/*----------------------------------------------------------------------------*/
void MainWindow::started( IDevice* device )
{
	Q_ASSERT (device);
	actionStart->setEnabled( false );
	actionStop->setEnabled( true );

	setWindowTitle( QString( UI_MW_TITLE_LISTEN ).arg( device->getName() ) );
}
/*----------------------------------------------------------------------------*/
void MainWindow::stopped( IDevice* device )
{
	Q_UNUSED (device);
	
	actionStart->setEnabled( true );
	actionStop->setEnabled( false );
	
	setWindowTitle( UI_MW_TITLE_OFF );
}
/*----------------------------------------------------------------------------*/
void MainWindow::printError(QString text)
{
	QMessageBox::critical(this, UI_NAME, text, QMessageBox::Ok);
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions()
{
	OptionsDialog opt(this);
  
//	connect( &opt, SIGNAL(newPlugin( QString )), ANALYZER, SLOT( addPlugin( QString )) );
  //connect( ANALYZER, SIGNAL(newPlugin( QPluginLoader* )), &opt, SLOT( addControl( QPluginLoader* )) );

	const OptionsList current = ANALYZER->registeredOptions();
	
	PRINT_DEBUG << "Adding option tabs: " << current;

	for (OptionsList::ConstIterator it = current.begin(); it != current.end(); ++it) 
		opt.addOptionsPage(*it);

  opt.exec();
}
/*----------------------------------------------------------------------------*/
void MainWindow::analyze(QModelIndex index)
{
	if (!index.isValid())
		index = view->currentIndex();

	if (!index.isValid()) // nonthing is selected
		return;
	
	QPointer<IConnection> con = ANALYZER->connection(index);
	
	if (!con) return; // somthing wen t wrong connection does not exist
	AnalyzeDialog dialog( this, con );
	dialog.exec();
}
/*----------------------------------------------------------------------------*/
void MainWindow::closeConnection()
{
	QModelIndex index = view->currentIndex();
	if (!index.isValid())
		return;
	QPointer<IConnection> con = ANALYZER->connection(index);
	if (!con) return; // somthing went wrong connection does not exist
	PRINT_DEBUG << "closing by request: " << con;
	con->close();
}
