#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>
#include <QListWidget>
#include <QObject>
#include <QtGui>
#include "ui_MainWindow.h"
#include "IConnection.h"
#include "IAnalyzer.h"

class MainWindow:public QMainWindow, private Ui_MainWindow {

	Q_OBJECT


public:
	MainWindow(IAnalyzer * controlledAnalyzer = NULL);
	bool attach(IAnalyzer * controlledAnalyzer);

private slots:
	void started(QString);
	void stopped(QString);
	bool connectDevice(IDevice * device);
	void setDevices(const QStringList newDevices);
	void snifferPlugin();	
	void showOptions();
	void printError(QString text);

signals:
	void newSniffer(QString path);

private:
	QComboBox * NICs;
	QMenu * deathMenu;

	void readSettings();
	void writeSettings();
	
	MainWindow(const MainWindow & win);
	const MainWindow& operator=(const MainWindow & win);

};
#endif
