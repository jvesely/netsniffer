#ifndef _CONTROL_H_
#define _CONTROL_H_


#include "ui_RecognizerLoad.h"
class Control:public QWidget, private Ui_Control {

	Q_OBJECT;	
	

private slots:
	void getFile();

public:
	Control(QWidget * parent);
	~Control();

public slots:
	void setStatus(QPair<QString, bool> status);

signals:
	void load();
	void unload();
	void remove();
	void setFile(QString file);
	
};

#endif
