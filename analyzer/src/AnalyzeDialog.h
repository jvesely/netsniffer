#pragma once

#include "ui_AnalyzeDialog.h"

class AnalyzeDialog:public QDialog, protected Ui::AnalyzeDialog {
	Q_OBJECT;
public:
	inline AnalyzeDialog() { setupUi(this); }
};
