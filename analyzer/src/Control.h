#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "ui_RecognizerLoad.h"
#include "IRecognizer.h"

class Control:public QWidget, private Ui_Control {

	Q_OBJECT;	
	

private slots:
	void getFile();

public:
	Control(QWidget * parent, IRecognizer * rec);
	~Control(){};

public slots:
	void setStatus(IRecognizer * rec);

signals:
	void setFile(QString file);
	
};

#endif
