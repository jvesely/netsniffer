#include "MainWindow.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"
#include "OptionsDialog.h"
#include "uitexts.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG_TEXT "[ Main Window ]: "
#include "debug.h"

MainWindow::MainWindow( IAnalyzer* analyzer )
{
	setupUi( this );

  { /* Creating main window icon. */
    QIcon icon;
    const int sizes[] = { 16, 32, 48 };

    for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
      icon.addPixmap(QString(":/icons/icon-%1.png").arg(sizes[i]));
    }

    qApp->setWindowIcon( icon );
  }

	NICs = new QComboBox( toolBar );
	Q_ASSERT( NICs );
	NICs->setSizeAdjustPolicy( QComboBox::AdjustToContents );
//
	//QToolButton * purgeButton = (QToolButton*)toolBar->widgetForAction(actionPurge);
	//deathMenu = NULL;
	//deathMenu = new QMenu(purgeButton);

	//purgeButton->setMenu(deathMenu);
	//purgeButton->setPopupMode(QToolButton::MenuButtonPopup);
	
	//deathMenu->addAction(actionAuto_Purge);

	toolBar->addWidget( NICs );
	//toolBar->addSeparator();
	//toolBar->addWidget(deathWarden);
	
	connect(actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()));
	connect(actionAnalyze, SIGNAL(triggered()), this, SLOT(analyze()));
	connect(actionCloseConnection, SIGNAL(triggered()), this, SLOT(closeConnection()));
	connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(analyze(QModelIndex)));
	
	if (analyzer)
		attach( analyzer );
	
	readSettings();
	show();
}
/*----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
	writeSettings();
}
/*----------------------------------------------------------------------------*/
bool MainWindow::attach( IAnalyzer* analyzer )
{
	Q_ASSERT( analyzer );
	PRINT_DEBUG << "Attaching analyzer.." << analyzer;
	
	m_analyzer = analyzer;
	view->setModel( analyzer->model() );
	PRINT_DEBUG << "Devices:" << analyzer->deviceNames();
	PRINT_DEBUG << "Connecting stuff..";

	connect( NICs, SIGNAL(currentIndexChanged( int )), analyzer, SLOT(selectDevice( int )) );
	connect( actionAuto_Purge, SIGNAL(triggered( bool )), analyzer, SLOT(setAutoPurge( bool )) );
	connect( analyzer, SIGNAL(deviceChanged( IDevice * )), this, SLOT(connectDevice( IDevice * )) );
	connect( analyzer, SIGNAL(devicesChanged( QStringList )), this, SLOT(setDevices( QStringList )) );
	connect( analyzer, SIGNAL(error( QString )), this, SLOT(printError( QString )) );
	
	setDevices( analyzer->deviceNames() );
	connectDevice( analyzer->currentDevice() );
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
/*
void MainWindow::loadPlugin()
{
	const QString path = QFileDialog::getOpenFileName(
			this, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ) );
	m_analyzer->addPlugin( path );
}
*/
/*----------------------------------------------------------------------------*/
void MainWindow::setDevices( const QStringList devices )
{

	NICs->clear();
	PRINT_DEBUG << "ComboBox cleared and ready for new items" << devices;
	if ( !devices.isEmpty() ) // there is something to select from
	{
		NICs->addItems( devices );
		NICs->setCurrentIndex( 0 ); // select first
	}
	PRINT_DEBUG << "Combobox done";
}
/*----------------------------------------------------------------------------*/
void MainWindow::readSettings()
{
	QSettings settings;

	QRect screen = QApplication::desktop()->availableGeometry();
	const QPoint center = screen.center();
	screen.setSize( QSize(900, 600) );
	screen.moveCenter( center );

	settings.beginGroup( QString("window") );
	const QSize size = settings.value("size", screen.size()).toSize();
	const QPoint pos = settings.value("pos", screen.topLeft()).toPoint();
	settings.endGroup();
	
	resize( size );
	move( pos );
}
/*----------------------------------------------------------------------------*/
void MainWindow::writeSettings()
{
	QSettings settings;
	
	settings.beginGroup( QString("window") );
	settings.setValue( "pos", pos() );
	settings.setValue( "size", size() );
	settings.endGroup();
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
void MainWindow::printError( const QString text )
{
	QMessageBox::critical( this, UI_NAME, text, QMessageBox::Ok );
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions()
{
	OptionsDialog opt( this );
  
	const OptionsList current = m_analyzer->registeredOptionPages();
	
	PRINT_DEBUG << "Adding option tabs: " << current;

	for (OptionsList::ConstIterator it = current.begin(); it != current.end(); ++it) 
		opt.addOptionsPage(*it);

  if( opt.exec() == QDialog::Accepted )
		m_analyzer->saveSettings();
}
/*----------------------------------------------------------------------------*/
void MainWindow::analyze( QModelIndex index )
{
	if (!index.isValid())
		index = view->currentIndex();

	if (!index.isValid()) // nonthing is selected
		return;
	
	//QPointer<IConnection> con = ANALYZER->connection(index);
	
//	if (!con) return; // somthing wen t wrong connection does not exist
//	AnalyzeDialog dialog( this, con );
//	dialog.exec();
}
/*----------------------------------------------------------------------------*/
void MainWindow::closeConnection()
{
	QModelIndex index = view->currentIndex();
	if (!index.isValid())
		return;
	IConnection* con = m_analyzer->connection( index );
	if (!con) return; // somthing went wrong connection does not exist
	PRINT_DEBUG << "closing by request: " << con;
	con->close();
}
