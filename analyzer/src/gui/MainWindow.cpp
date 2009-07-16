#include "MainWindow.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "OptionsDialog.h"
#include "uitexts.h"


#define DEBUG_TEXT "[ Main Window ]: "
#include "debug.h"

MainWindow::MainWindow( IAnalyzer* analyzer )
{
	setupUi( this );


	NICs = new QComboBox( toolBar );
	Q_ASSERT( NICs );
	NICs->setSizeAdjustPolicy( QComboBox::AdjustToContents );
	toolBar->addWidget( NICs );

	connect( view, SIGNAL(customContextMenuRequested( const QPoint& )),
		this, SLOT(displayMenu( const QPoint& )) );
	connect( actionOptions, SIGNAL(triggered()), this, SLOT(showOptions()) );
	connect( actionDetails, SIGNAL(triggered()), this, SLOT(showDetails()) );
	connect( view, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showDetails()) );
	connect( actionSetRecognizer, SIGNAL(triggered()), this, SLOT(setRecognizer()) );
	connect( actionRemoveDead, SIGNAL(triggered()), this, SLOT(removeConnections()) );
	connect( actionAutoRemove, SIGNAL(toggled( bool )), this, SLOT(removeConnections( bool )) );

	/* map closing signal */
	QSignalMapper* mapper =  new QSignalMapper( this );
	Q_ASSERT (mapper);
	mapper->setMapping( actionCloseConnection, 0 );
	mapper->setMapping( actionCloseAllConnections, 1 );

	connect( actionCloseConnection, SIGNAL(triggered()), mapper, SLOT(map()) );
	connect( actionCloseAllConnections, SIGNAL(triggered()), mapper, SLOT(map()) );
	connect( mapper, SIGNAL(mapped( int )), this, SLOT( closeConnection( int )));
	mapper = NULL;
	
	/* map killing signal */
	mapper = new QSignalMapper( this );
	Q_ASSERT (mapper);
	mapper->setMapping( actionKillConnection, 0 );
	mapper->setMapping( actionKillAllConnections, 1 );

	connect( actionKillConnection, SIGNAL(triggered()), mapper, SLOT(map()) );
	connect( actionKillAllConnections, SIGNAL(triggered()), mapper, SLOT(map()) );
	connect( mapper, SIGNAL(mapped( int )), this, SLOT( killConnection( int )));

	if (analyzer)
		attach( analyzer );
	
	loadSettings();
	show();
}
/*----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
	saveSettings();
	delete mModel;
}
/*----------------------------------------------------------------------------*/
bool MainWindow::attach( IAnalyzer* analyzer )
{
	Q_ASSERT( analyzer );
	PRINT_DEBUG ("Attaching analyzer.." << analyzer);
	
	mAnalyzer = analyzer;
	mModel = new ConnectionModel( analyzer );
	Q_ASSERT( mModel );
	view->setModel( mModel );
	connect( mModel, SIGNAL(connectionCount( int )), this, SLOT(refreshStatusBar()) );

	PRINT_DEBUG ("Devices:" << analyzer->deviceNames());
	PRINT_DEBUG ("Connecting stuff..");

	connect( NICs, SIGNAL(currentIndexChanged( int )), analyzer, SLOT(selectDevice( int )) );
	connect( analyzer, SIGNAL(deviceChanged( IDevice* )), this, SLOT(connectDevice( IDevice* )) );
	connect( analyzer, SIGNAL(devicesChanged( QStringList )), this, SLOT(setDevices( QStringList )) );
	connect( analyzer, SIGNAL(error( QString )), this, SLOT(printError( QString )) );
	connect( analyzer, SIGNAL(newConnection( IConnection::Pointer )),
		this, SLOT( newConnection( IConnection::Pointer )) );
	
	setDevices( analyzer->deviceNames() );
	return true;
}
/*----------------------------------------------------------------------------*/
void MainWindow::displayMenu( const QPoint& pos )
{
	Q_UNUSED (pos);
	menuConnection->exec( QCursor::pos() );
}
/*----------------------------------------------------------------------------*/
void MainWindow::refreshStatusBar()
{
	if ( mAnalyzer && mAnalyzer->currentDevice() )
	{
		IDevice::Stats stats = mAnalyzer->currentDevice()->getStats();
		using namespace UiTexts;
		static const QString format = QString( "%1 %2 %3 %4 %5" ).arg( INTERFACE_COUNT, CONNECTIONS_COUNT, RECIEVED_PACKETS_COUNT, DROPPED_PACKETS_COUNT, PACKETS_DROPPED_INTERFACE );
		const QString message =
			format.arg( NICs->count() ).arg( mModel->rowCount( QModelIndex() ) ).arg( stats.received ).arg( stats.dropped ).arg( stats.ifdropped );
		statusBar()->showMessage( message );

	}
}
/*----------------------------------------------------------------------------*/
bool MainWindow::connectDevice( IDevice* device )
{
	if ( !device )
		return false;
	PRINT_DEBUG ("Connecting device..." << device);

	return 
	   connect( device, SIGNAL(captureStarted( IDevice* )), this, SLOT(started( IDevice* )) )
	&& connect( device, SIGNAL(captureStopped( IDevice* )), this, SLOT(stopped( IDevice* )) )
	&& connect( device, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(refreshStatusBar()) )
	&& connect( actionStart, SIGNAL(triggered()), device, SLOT(captureStart()) )
	&& connect( actionStop, SIGNAL(triggered()), device, SLOT(captureStop()) );
}
/*----------------------------------------------------------------------------*/
void MainWindow::loadPlugin()
{
	const QString path = QFileDialog::getOpenFileName(
			this, tr( UiTexts::PLUGIN_LOAD ), QApplication::applicationDirPath(), tr( UiTexts::PLUGINS_SUFFIX ) );
	mAnalyzer->addPlugin( path );
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
void MainWindow::loadSettings()
{
	QRect screen = QApplication::desktop()->availableGeometry();
	const QPoint center = screen.center();
	screen.setSize( QSize(900, 600) );
	screen.moveCenter( center );

	QSettings settings;

	settings.beginGroup( "window" );
	const QSize size = settings.value( "size", screen.size() ).toSize();
	const QPoint pos = settings.value( "pos", screen.topLeft() ).toPoint();
	const QString activeNIC = settings.value( "activeNIC" ).toString();
	const bool autoRemove = settings.value( "autoRemoveDead", true ).toBool();
	settings.endGroup();

	resize( size );
	move( pos );

	Q_ASSERT (actionAutoRemove);
	actionAutoRemove->setChecked( autoRemove );

	Q_ASSERT (NICs);
	const int index = NICs->findText( activeNIC );
	if (index != -1) {
		NICs->setCurrentIndex( index );
	}
}
/*----------------------------------------------------------------------------*/
void MainWindow::saveSettings()
{
	Q_ASSERT (NICs);
	Q_ASSERT (actionAutoRemove);
	QSettings settings;
	
	settings.beginGroup( QString("window") );
	settings.setValue( "pos", pos() );
	settings.setValue( "size", size() );
	settings.setValue( "activeNIC", NICs->currentText() );
	settings.setValue( "autoRemoveDead", actionAutoRemove->isChecked() );
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
void MainWindow::printError( const QString& text )
{
	QMessageBox::critical( this, QCoreApplication::applicationName(), text, QMessageBox::Ok );
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions()
{
	OptionsDialog opt( this );
  
	const IAnalyzer::OptionsList current = mAnalyzer->registeredOptionTabs();
	
	PRINT_DEBUG ("Adding option tabs: " << current);

	for (IAnalyzer::OptionsList::ConstIterator it = current.begin();
		it != current.end(); ++it) 
	{ 
		opt.addOptionsTab( *it );
	}

  if( opt.exec() == QDialog::Accepted )
		mAnalyzer->saveSettings();
}
/*----------------------------------------------------------------------------*/
void MainWindow::showDetails()
{
	QModelIndex index = view->currentIndex();

	if (!index.isValid()) // nothing is selected
		return;

	Q_ASSERT( mModel );
	IConnection::Pointer connection = mModel->connection( index );
	Q_ASSERT( connection );
	connection->showDetails();
}
/*----------------------------------------------------------------------------*/
void MainWindow::closeConnection( int all, bool kill )
{
	ConnectionList list;
	if (!all)
	{
		const QModelIndex index = view->currentIndex();
		if (!index.isValid()) // nothing is selected
			return;
		Q_ASSERT( mModel );
		list << mModel->connection( index );
	} else {
		list = mAnalyzer->connections();
	}
	
	while ( !list.isEmpty() )
	{
		IConnection::Pointer connection = list.takeFirst();
		Q_ASSERT( connection );

		connection->close();
		if (kill)
			connection->die();
	}
}
/*----------------------------------------------------------------------------*/
void MainWindow::killConnection( int all )
{
	PRINT_DEBUG( "Killing" << (all ? "all" : "one") );
	closeConnection( all, true );
}
/*----------------------------------------------------------------------------*/
void MainWindow::newConnection( IConnection::Pointer connection )
{
	connect(
		connection.data(),
		SIGNAL( statusChanged( IConnection::Pointer, IConnection::Status )),
		this,
		SLOT( updateConnection( IConnection::Pointer, IConnection::Status ))
	);
	Q_ASSERT (mModel);
	mModel->addConnection( connection );
}
/*----------------------------------------------------------------------------*/
void MainWindow::updateConnection( IConnection::Pointer connection, IConnection::Status status )
{
	Q_ASSERT (mModel);
	Q_ASSERT (actionAutoRemove);
	
	if (status == IConnection::Dead && actionAutoRemove->isChecked())
	{
		PRINT_DEBUG( "UPDATING DEAD CONNECTION" );
		mModel->removeConnection( connection );
		mAnalyzer->removeConnection( connection );
	} else {
		PRINT_DEBUG( "UPDATING LIVE CONNECTION" );
		mModel->updateConnection( connection );
	}
}
/*----------------------------------------------------------------------------*/
void MainWindow::removeConnections( bool remove )
{
	if (!remove)
		return;

	ConnectionList list = mAnalyzer->connections();
	
	while ( !list.isEmpty() )
	{
		IConnection::Pointer connection = list.takeFirst();
		Q_ASSERT( connection );

		if (connection->status() == IConnection::Dead)
		{
			mModel->removeConnection( connection );
			mAnalyzer->removeConnection( connection );
		}
	}
}
/*----------------------------------------------------------------------------*/
void MainWindow::setRecognizer()
{
	const QModelIndex index = view->currentIndex();
	if (!index.isValid())
		return;
	const IAnalyzer::RecognizerList list = mAnalyzer->registeredRecognizers();
	QStringList recognizers;
	for (IAnalyzer::RecognizerList::const_iterator it = list.begin();
		it != list.end(); ++it )
	{
		recognizers << (*it)->name();
	}
	recognizers << "No Recognizer";

	IConnection::Pointer connection = mModel->connection( index );

	bool ok = false;
	const QString selected = QInputDialog::getItem( this, "Select recognizer", "Recognizers:", recognizers, 0, false, &ok );

	if (ok)
	{
		const int index = recognizers.indexOf( selected );
		if ( index >= list.count() || index < 0 )
		{
			connection->setRecognizer( NULL );
		} else {
			connection->setRecognizer( list[index] );
		}
	}
}
/*----------------------------------------------------------------------------*/
