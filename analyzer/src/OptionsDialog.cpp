#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include "OptionsDialog.h"
#include "Control.h"

OptionsDialog::OptionsDialog(QWidget * parent):QDialog(parent){
	setupUi(this);
	connect(pushButtonAdd, SIGNAL(clicked()), this, SLOT(addModule()));
	connect(pushButtonDiscard, SIGNAL(clicked()), this, SLOT(discard()));
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addModule() {
	QString module = QFileDialog::getOpenFileName(this, tr("Load new module"), ".", "Recognizing engines (*.so *.dll)");
	if (!module.isEmpty())
		emit newModule(module);
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::discard(){
	int ret = QMessageBox::warning(this, tr("Delete ALL modules?"), tr("This action will DELETE ALL recognizing modules. Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes)
		emit discardModules();
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::addControl(Recognizer * rec){
	Control * newCtrl = new Control(this);
	newCtrl->setStatus(rec->getStatus());
	connect(newCtrl, SIGNAL(load()), rec, SLOT(load()));
	connect(newCtrl, SIGNAL(unload()), rec, SLOT(unload()));
	connect(newCtrl, SIGNAL(setFile(QString)), rec, SLOT(setFile(QString)));
	connect(newCtrl, SIGNAL(remove()), rec, SLOT(deleteLater()));
	connect(rec, SIGNAL(statusChanged(QPair<QString, bool>)), newCtrl, SLOT(setStatus(QPair<QString, bool>)));
	connect(rec, SIGNAL(destroyed()), newCtrl, SLOT(deleteLater()));
	QVBoxLayout * layout = (QVBoxLayout *)groupBoxRecognizers->layout();
	layout->insertWidget(layout->count() - 1, newCtrl);
}
