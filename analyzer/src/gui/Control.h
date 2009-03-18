#pragma once

#include "IRecognizer.h"
#include "ui_RecognizerLoad.h"

class Control:public QWidget, private Ui_Control
{

	Q_OBJECT;	
public:
	Control( QWidget * parent, QPluginLoader * loader );
	~Control(){};

signals:
	void setFile( QString file );
	
private slots:
	void getFile();
	void updateStatus();
	void switchStatus();

private:
	QPointer<QPluginLoader> m_plugin;

};

