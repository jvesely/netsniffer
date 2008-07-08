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
	void display(Connection * con, bool fresh = false);

signals:
	void newSniffer();
	void selectNIC(int num);
	void startNIC();
	void stopNIC();
	void autoPurge(bool on);
	void purge();

private:
	ConnectionModel * store;
	QComboBox * NICs;
	QMenu * deathMenu;

	void readSettings();
	void writeSettings();
	MainWindow(const MainWindow & win);
	const MainWindow& operator=(const MainWindow & win);

};
#endif
