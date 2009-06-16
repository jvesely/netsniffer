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
	connect( actionAutoRemove, SIGNAL(triggered( bool )), analyzer, SLOT(setAutoPurge( bool )) );
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
void MainWindow::loadPlugin()
{
	const QString path = QFileDialog::getOpenFileName(
			this, tr( UiTexts::PLUGIN_LOAD ), ".", tr( UiTexts::PLUGINS_SUFFIX ) );
	m_analyzer->addPlugin( path );
}
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

	QString selected = QInputDialog::getItem( this, "Select recognizer", "Recognizers:", recognizers, 0 );
	QMessageBox::information( this, "Selected", "You selected:" + selected );
}
/*----------------------------------------------------------------------------*/
void MainWindow::readSettings()
{
	QRect screen = QApplication::desktop()->availableGeometry();
	const QPoint center = screen.center();
	screen.setSize( QSize(900, 600) );
	screen.moveCenter( center );

	QSettings settings;

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

	setWindowTitle( QString( UiTexts::MW_TITLE_LISTEN ).arg( device->getName() ) );
}
/*----------------------------------------------------------------------------*/
void MainWindow::stopped( IDevice* device )
{
	Q_UNUSED (device);
	
	actionStart->setEnabled( true );
	actionStop->setEnabled( false );
	
	setWindowTitle( UiTexts::MW_TITLE_OFF );
}
/*----------------------------------------------------------------------------*/
void MainWindow::printError( const QString text )
{
	QMessageBox::critical( this, UiTexts::NAME, text, QMessageBox::Ok );
}
/*----------------------------------------------------------------------------*/
void MainWindow::showOptions()
{
	OptionsDialog opt( this );
  
	const IAnalyzer::OptionsList current = m_analyzer->registeredOptionTabs();
	
	PRINT_DEBUG << "Adding option tabs: " << current;

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

	if (!index.isValid()) // nonthing is selected
		return;
	
	m_analyzer->connectionDetail( index );
}
/*----------------------------------------------------------------------------*/
void MainWindow::closeConnection()
{
	QModelIndex index = view->currentIndex();
	m_analyzer->connectionClose( index );
}
