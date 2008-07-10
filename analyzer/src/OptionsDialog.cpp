#include <QFileDialog>
#include <QMessageBox>

#include "OptionsDialog.h"


OptionsDialog::OptionsDialog(QWidget * parent):QDialog(parent){
	setupUi(this);
	connect(pushButtonAdd, SIGNAL(clicked()), this, SLOT(addModule()));
	connect(pushButtonDiscard, SIGNAL(clicked()), this, SLOT(discard()));
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addModule() {
	QString module = QFileDialog::getOpenFileName(this, tr("Load new module"), ".", "Recognizing engines (*.so *.dll)");
	emit newModule(module);
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::discard(){
	int ret = QMessageBox::warning(this, tr("Delete ALL modules?"), tr("This action will DELETE ALL recognizing modules. Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes)
		emit discardModules();
}
