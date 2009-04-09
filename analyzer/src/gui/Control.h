#pragma once

//#include "IRecognizer.h"
#include "ui_RecognizerLoad.h"
#include "PluginLoader.h"


class Control:public QWidget, private Ui_Control
{

	Q_OBJECT;	
public:
	Control( QWidget * parent, PluginLoader * loader );
	~Control(){};

signals:
	void setFile( QString file );
	
public slots:
	void getFile();
	void updateStatus( bool loaded );
	void switchStatus();

private:
	QPointer<PluginLoader> m_plugin;
};
