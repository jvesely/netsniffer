#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>
#include <QListWidget>
#include "IDevList.h"
#include "cAnalyzer.h"



class mainWindow:public QMainWindow{

Q_OBJECT

public:
  mainWindow();

public slots:
	void print(QString text);
	void stop();
	void start();
	void started();
	void stopped();
	
	

signals:

private:
	cAnalyzer * analyzer;
	QListWidget * view;
	QMenu *devMenu;
	QAction * startCapture;
	QAction * stopCapture;

	QObject * loadPlugin(QString path);
	void createActions();
	void createMenus();
	void readSettings();
	void writeSettings();

};
#endif
