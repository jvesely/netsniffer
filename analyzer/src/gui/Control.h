#pragma once

//#include "IRecognizer.h"
#include "ui_RecognizerLoad.h"
#include "PluginLoader.h"


class Control:public QWidget, private Ui::Control
{
public:
	Control( QWidget* parent, PluginLoader* loader );
	~Control() {};

public slots:
	void updateStatus( bool loaded );
	void switchStatus();

private:
	Q_OBJECT

	QPointer<PluginLoader> m_plugin;
};
