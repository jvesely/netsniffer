#include <QFileDialog>
#include <QHBoxLayout>

#include "Control.h"
#include <QDebug>
#include <QPair>
#include <QFileInfo>

Control::Control(QWidget * parent, IRecognizer * rec):QWidget(parent){
	setupUi(this);
	setStatus(rec);
	//my signals
	connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(getFile()));
	// accpets
	connect(rec, SIGNAL(statusChanged(IRecognizer *)), this, SLOT(setStatus(IRecognizer *)));
	connect(rec, SIGNAL(destroyed()), this, SLOT(deleteLater()));
	//sends
	connect(this, SIGNAL(setFile(QString)), rec, SLOT(setFile(QString)));
	connect(pushButtonRemove, SIGNAL(clicked()), rec, SLOT(deleteLater()));


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
void Control::setStatus(IRecognizer * me) {
	//qDebug() << "setting status" << status;
	QString filename = me->fileName();
	labelPath->setText(QFileInfo(filename).baseName());
	labelPath->setToolTip(filename);
	if(me->isLoaded()){
		pushButtonLoad->setText("&Unload");
		pushButtonLoad->setIcon(QIcon(":/control/unload.png"));
		disconnect(pushButtonLoad, 0, 0, 0);
		connect(pushButtonLoad, SIGNAL(clicked()), me, SLOT(unload()));
		labelStatus->setPixmap(QPixmap(":/control/ok.png"));
	}else{
		disconnect(pushButtonLoad, 0, 0, 0);
		connect(pushButtonLoad, SIGNAL(clicked()), me, SLOT(load()));
		pushButtonLoad->setText("&Load");
		pushButtonLoad->setIcon(QIcon(":/control/load.png"));
		labelStatus->setPixmap(QPixmap(":/control/error.png"));
	}
}
/*----------------------------------------------------------------------------*/
