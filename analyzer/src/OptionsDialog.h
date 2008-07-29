#pragma once

#include "ui_OptionsDialog.h"
#include "IRecognizer.h"

class OptionsDialog:public QDialog, protected Ui_OptionsDialog{

	Q_OBJECT;
	
protected slots:
	void addModule();
	void discard();
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent * event);

public:
	OptionsDialog(QWidget * parent);


public slots:
	void addControl(IRecognizer * rec);

signals:
	void newModule(QString path);
	void discardModules();
		
};
