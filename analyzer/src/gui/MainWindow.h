/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "ui_MainWindow.h"
#include "ConnectionModel.h"

class IDevice;
class IAnalyzer;

class MainWindow: public QMainWindow, private Ui::MainWindow
{

	Q_OBJECT

public:
	MainWindow( IAnalyzer* analyzer = NULL );
	~MainWindow();
	bool attach( IAnalyzer* analyzer );

public slots:
	void displayMenu( const QPoint& pos );

private slots:
	void started( IDevice* );
	void stopped( IDevice* );

	bool connectDevice( IDevice* device );
	void refreshStatusBar();
	void setRecognizer();
	void setDevices( const QStringList newDevices );
	void loadPlugin();	
	void showOptions();
	void showDetails();
	void printError( const QString& text );

	void closeConnection( int all = 0, bool kill = false );
	void killConnection( int all = 0 );
	void newConnection( IConnection::Pointer connection );
	void updateConnection( IConnection::Pointer connection, IConnection::Status status );
	void removeConnections( bool remove = true );

private:
	QPointer<QComboBox> NICs;
	QPointer<ConnectionModel> mModel;
	IAnalyzer* mAnalyzer;

	void loadSettings();
	void saveSettings();
	
	Q_DISABLE_COPY (MainWindow);
};
