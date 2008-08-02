#include "AnalyzeDialog.h"


AnalyzeDialog::AnalyzeDialog(QWidget* parent, IConnection * con):QDialog(parent) {
	setupUi(this);
	connection = con;
}
