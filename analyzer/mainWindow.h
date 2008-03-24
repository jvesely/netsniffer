#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>
#include <QListWidget>
#include <QObject>
#include <QtGui>
#include "ui_mainWindow.h"
#include "IDevList.h"
#include "CAnalyzer.h"


class mainWindow:public QMainWindow, private Ui_MainWindow {

Q_OBJECT

public:
  mainWindow();

public slots:
	void print(QString text);
	void stop();
	void start();
	void started(QString);
	void stopped(QString);
	void loadSniffer();	

private:
	CAnalyzer * analyzer;
	QComboBox * NICs;

	QObject * loadPlugin(QString path);
	void readSettings();
	void writeSettings();

};
#endif
