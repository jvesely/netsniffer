#ifndef _OPTIONSWINDOW_H_
#define _OPTIONSWINDOW_H_

#include "ui_OptionsDialog.h"
#include "IRecognizer.h"

class OptionsDialog:public QDialog, protected Ui_OptionsDialog{

	Q_OBJECT;
	
protected slots:
	void addModule();
	void discard();

public:
	OptionsDialog(QWidget * parent);

public slots:
	void addControl(IRecognizer * rec);

signals:
	void newModule(QString path);
	void discardModules();
		
};

#endif
