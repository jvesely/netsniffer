#pragma once

#include "ui_MainWindow.h"

class IDevice;
class IAnalyzer;

class MainWindow: public QMainWindow, private Ui_MainWindow
{

	Q_OBJECT

public:
	MainWindow( IAnalyzer* analyzer = NULL );
	~MainWindow();
	bool attach( IAnalyzer* analyzer );

private slots:
	void started( IDevice* );
	void stopped( IDevice* );

	bool connectDevice( IDevice* device );
	void closeConnection();
	void setDevices( const QStringList newDevices );
	void loadPlugin();	
	void showOptions();
	void showDetails( QModelIndex = QModelIndex() );
	void printError( const QString text );

private:
	QComboBox* NICs;
//	QMenu* deathMenu;
	IAnalyzer* m_analyzer;

	void readSettings();
	void writeSettings();
	
	Q_DISABLE_COPY (MainWindow);
};
