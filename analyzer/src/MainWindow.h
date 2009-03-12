#pragma once

#include "ui_MainWindow.h"

class IDevice;

class MainWindow: public QMainWindow, private Ui_MainWindow
{

	Q_OBJECT


public:
	MainWindow();
	bool attach();

private slots:
	void started(QString);
	void stopped(QString);
	bool connectDevice(IDevice * device);
	void closeConnection();
	void setDevices(const QStringList newDevices);
	void snifferPlugin();	
	void showOptions();
	void analyze(QModelIndex = QModelIndex());
	void printError(QString text);

signals:
	void newSniffer(QString path);

private:
	QComboBox * NICs;
	QMenu * deathMenu;

	void readSettings();
	void writeSettings();
	
	Q_DISABLE_COPY (MainWindow);
};
