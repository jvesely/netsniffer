#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>
#include <QListWidget>
#include <QObject>
#include <QtGui>
#include "ui_MainWindow.h"
#include "ConnectionModel.h"


class MainWindow:public QMainWindow, private Ui_MainWindow {

Q_OBJECT

public:
  MainWindow();

public slots:
	void started(QString dev);
	void stopped(QString dev);
	QString getPlugin(QString path = QString());	
	void setSelector(QStringList devs);
	void display(Connection * con);

signals:
	void newPlugin(QString path);
	void selectNIC(int num);
	void startNIC();
	void stopNIC();

private:
	ConnectionModel * store;
	QComboBox * NICs;

	QObject * loadPlugin(QString path);
	void readSettings();
	void writeSettings();

};
#endif
