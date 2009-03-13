#pragma once

#include "ui_OptionsDialog.h"

class IOptionsPage;

class OptionsDialog:public QDialog, protected Ui_OptionsDialog
{

	Q_OBJECT;
	
public:
	OptionsDialog(QWidget * parent);

public slots:
		void addOptionsPage( IOptionsPage* new_page );
//	void addControl(QPluginLoader * plugin);

protected slots:
//	void addPlugin();
//	void discard();
//	void dropEvent(QDropEvent* event);
//	void dragEnterEvent(QDragEnterEvent * event);


signals:
//	void newPlugin(QString path);
//	void discardPlugins();
		
};
