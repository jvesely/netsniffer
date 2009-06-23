#include "MainWindow.h"
#include "IAnalyzer.h"
#include "IDevice.h"
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
	
	view->addAction( actionDetails );
	view->addAction( actionCloseConnection );
	view->addAction( actionRemoveDead );
	view->addAction( actionSetRecognizer );

	toolBar->addWidget( NICs );
	
	connect( actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()) );
	connect( actionDetails, SIGNAL(triggered()), this, SLOT(showDetails()) );
	connect( actionCloseConnection, SIGNAL(triggered()), this, SLOT(closeConnection()) );
	connect( actionSetRecognizer, SIGNAL(triggered()), this, SLOT(setRecognizer()) );
	connect( view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDetails()) );
	//connect( actionStart, SIGNAL(triggered()), this, SLOT(refreshStatusBar()) );
	connect( actionStop, SIGNAL(triggered()), this, SLOT(refreshStatusBar()) );
	
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
	PRINT_DEBUG ("Attaching analyzer.." << analyzer);
	
	m_analyzer = analyzer;
	m_model = new ConnectionModel( analyzer );
	Q_ASSERT( m_model );
	view->setModel( m_model );

	PRINT_DEBUG ("Devices:" << analyzer->deviceNames());
	PRINT_DEBUG ("Connecting stuff..");

	connect( NICs, SIGNAL(currentIndexChanged( int )), analyzer, SLOT(selectDevice( int )) );
	connect( actionAutoRemove, SIGNAL(triggered( bool )), analyzer, SLOT(setAutoPurge( bool )) );
	connect( analyzer, SIGNAL(deviceChanged( IDevice * )), this, SLOT(connectDevice( IDevice * )) );
	connect( analyzer, SIGNAL(devicesChanged( QStringList )), this, SLOT(setDevices( QStringList )) );
	connect( analyzer, SIGNAL(error( QString )), this, SLOT(printError( QString )) );
	
	setDevices( analyzer->deviceNames() );
	connectDevice( analyzer->currentDevice() );
	return true;
}
/*----------------------------------------------------------------------------*/
void MainWindow::refreshStatusBar()
{
	if ( m_analyzer && m_analyzer->currentDevice() )
	{
		//IDevice::Stats stats = m_analyzer->currentDevice()->getStats();
		//QString message = QString( "RECEVIED PACKETS: %1 DROPPED PACKETS: %2" ).arg( stats.received ).arg( stats.dropped );
		//QMessageBox::information( this, "foo", message, QMessageBox::Ok );
	}
}
/*----------------------------------------------------------------------------*/
bool MainWindow::connectDevice( IDevice* device )
{
	if ( !device )
		return false;
	PRINT_DEBUG ("Connecting device...");

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
			this, tr( UiTexts::PLUGIN_LOAD ), QApplication::applicationDirPath(), tr( UiTexts::PLUGINS_SUFFIX ) );
	m_analyzer->addPlugin( path );
}
/*----------------------------------------------------------------------------*/
void MainWindow::setDevices( const QStringList devices )
{
	NICs->clear();
	PRINT_DEBUG ("ComboBox cleared and ready for new items" << devices);
	if ( !devices.isEmpty() ) // there is something to select from
	{
		NICs->addItems( devices );
		NICs->setCurrentIndex( 0 ); // select first
	}
	PRINT_DEBUG ("Combobox done");
}
/*----------------------------------------------------------------------------*/
void MainWindow::readSettings()
{
	QRect screen = QApplication::desktop()->availableGeometry();
	const QPoint center = screen.center();
	screen.setSize( QSize(900, 600) );
	screen.moveCenter( center );

	QSettings settings;

	settings.beginGroup( "window" );
	const QSize size = settings.value("size", screen.size()).toSize();
	const QPoint pos = settings.value("pos", screen.topLeft()).toPoint();
	const QString activeNIC = settings.value("activeNIC").toString();
	settings.endGroup();

	resize( size );
	move( pos );

	const int index = NICs->findText( activeNIC );
	if (index != -1) {
		NICs->setCurrentIndex( index );
	}
}
/*----------------------------------------------------------------------------*/
void MainWindow::writeSettings()
{
	QSettings settings;
	
	settings.beginGroup( QString("window") );
	settings.setValue( "pos", pos() );
	settings.setValue( "size", size() );
	settings.setValue( "activeNIC", NICs->currentText() );
	settings.endGroup();
}
/*----------------------------------------------------------------------------*/
void MainWindow::started( IDevice* device )
{
	Q_ASSERT (device);
	actionStart->setEnabled( false );
	actionStop->setEnabled( true );

	QString name = device->getName();
	if (!device->getDescription().isEmpty()) {
		name += " (" + device->getDescription() + ")";
	}

	setWindowTitle( QString( "%1 listening on %2" ).arg( QApplication::applicationName(), name ) );
}
/*----------------------------------------------------------------------------*/
void MainWindow::stopped( IDevice* device )
{
	Q_UNUSED (device);
	
	actionStart->setEnabled( true );
	actionStop->setEnabled( false );
	
	setWindowTitle( QApplication::applicationName() );
}
/*----------------------------------------------------------------------------*/
void MainWindow::printError( const QString text )
{
	QMessageBox::critical( this, QCoreApplication::applicationName(), text, QMessageBox::Ok );
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions()
{
	OptionsDialog opt( this );
  
	const IAnalyzer::OptionsList current = m_analyzer->registeredOptionTabs();
	
	PRINT_DEBUG ("Adding option tabs: " << current);

	for (IAnalyzer::OptionsList::ConstIterator it = current.begin();
		it != current.end(); ++it) 
	{ 
		opt.addOptionsTab( *it );
	}

  if( opt.exec() == QDialog::Accepted )
		m_analyzer->saveSettings();
}
/*----------------------------------------------------------------------------*/
void MainWindow::showDetails()
{
	QModelIndex index = view->currentIndex();

	if (!index.isValid()) // nothing is selected
		return;

	Q_ASSERT( m_model );
	IConnection::Pointer connection = m_model->connection( index );
	Q_ASSERT( connection );
	connection->showDetails();
}
/*----------------------------------------------------------------------------*/
void MainWindow::closeConnection()
{
	QModelIndex index = view->currentIndex();
	if (!index.isValid()) // nothing is selected
		return;
	Q_ASSERT( m_model );
	IConnection::Pointer connection = m_model->connection( index );
	Q_ASSERT( connection );
	connection->close();
}
/*----------------------------------------------------------------------------*/
void MainWindow::setRecognizer()
{
	const QModelIndex index = view->currentIndex();
	if (!index.isValid())
		return;
	IAnalyzer::RecognizerList list = m_analyzer->registeredRecognizers();
	QStringList recognizers;
	for (IAnalyzer::RecognizerList::iterator it = list.begin();
		it != list.end(); ++it )
	{
		recognizers << (*it)->name();
	}
	recognizers << "No Recognizer";

	IConnection::Pointer connection = m_model->connection( index );

	bool ok = false;
	const QString selected = QInputDialog::getItem( this, "Select recognizer", "Recognizers:", recognizers, 0, false, &ok );

	if (ok)
	{
		const int index = recognizers.indexOf( selected );
		if ( index >= list.count() || index < 0 ) {
			connection->setRecognizer( NULL );
		} else {
			connection->setRecognizer( list[index] );
		}

	}
}
/*----------------------------------------------------------------------------*/
