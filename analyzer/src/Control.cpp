#include <QFileDialog>
#include <QHBoxLayout>

#include "Control.h"
#include <QDebug>
#include <QPair>
#include <QFileInfo>

Control::Control(QWidget * parent):QWidget(parent){
	setupUi(this);
	connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(getFile()));
	connect(pushButtonLoad, SIGNAL(clicked()), this, SIGNAL(load()));
	connect(pushButtonRemove, SIGNAL(clicked()), this, SIGNAL(remove()));
}
/*----------------------------------------------------------------------------*/
void Control::getFile(){
	QString module = QFileDialog::getOpenFileName(this, tr("Load new module"), ".", "Recognizing engines (*.so *.dll)");
	if (!module.isEmpty()){
		labelPath->setText(module);
		emit setFile(module);
	}
}
/*----------------------------------------------------------------------------*/
void Control::setStatus(QPair<QString, bool> status) {
	//qDebug() << "setting status" << status;
	labelPath->setText(QFileInfo(status.first).baseName());
	labelPath->setToolTip(status.first);
	if(status.second){
		pushButtonLoad->setText("&Unload");
		pushButtonLoad->setIcon(QIcon(":/control/unload.png"));
		disconnect(pushButtonLoad, 0, 0, 0);
		connect(pushButtonLoad, SIGNAL(clicked()), this, SIGNAL(unload()));
		labelStatus->setPixmap(QPixmap(":/control/ok.png"));

	}else{
		disconnect(pushButtonLoad, 0, 0, 0);
		connect(pushButtonLoad, SIGNAL(clicked()), this, SIGNAL(load()));
		pushButtonLoad->setText("&Load");
		pushButtonLoad->setIcon(QIcon(":/control/load.png"));
		labelStatus->setPixmap(QPixmap(":/control/error.png"));
	}
}
Control::~Control(){
	qDebug() << "deleting control";
}
