#include "IAnalyzer.h"
#include "OptionsDialog.h"
#include "Control.h"

OptionsDialog::OptionsDialog(QWidget * parent):QDialog(parent){
	setupUi(this);
	connect(pushButtonAdd, SIGNAL(clicked()), this, SLOT(addModule()));
	connect(pushButtonDiscard, SIGNAL(clicked()), this, SLOT(discard()));
	IAnalyzer * app = IAnalyzer::instance();
	connect(buttonBox, SIGNAL(accepted()), app, SLOT(saveSettings()));
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
void OptionsDialog::addControl(IRecognizer * rec){
	qDebug() << "Adding rercognizer control";
	Control * newCtrl = new Control(this, rec);
	connect(this, SIGNAL(discardModules()), rec, SLOT(deleteLater()));
	QVBoxLayout * layout = (QVBoxLayout *)scrollAreaWidgetContents->layout();
	layout->insertWidget(layout->count() - 1, newCtrl); // add right before the spacer
}
