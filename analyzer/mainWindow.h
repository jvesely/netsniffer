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
	void started(QString dev);
	void stopped(QString dev);
	void loadSniffer(QString path);	
	void setSelector(QStringList devs);

private:
	CAnalyzer * analyzer;
	QComboBox * NICs;

	QObject * loadPlugin(QString path);
	void readSettings();
	void writeSettings();

};
#endif
