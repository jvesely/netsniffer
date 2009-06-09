#pragma once

#include "ui_MainWindow.h"

class IDevice;

class MainWindow: public QMainWindow, private Ui_MainWindow
{

	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	bool attach();

private slots:
	void started( IDevice* );
	void stopped( IDevice* );

	bool connectDevice( IDevice* device );
	void closeConnection();
	void setDevices( const QStringList newDevices );
	void loadPlugin();	
	void showOptions();
	void analyze( QModelIndex = QModelIndex() );
	void printError( const QString text );

private:
	QComboBox* NICs;
	QMenu* deathMenu;

	void readSettings();
	void writeSettings();
	
	Q_DISABLE_COPY (MainWindow);
};
