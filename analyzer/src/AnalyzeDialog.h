#ifndef _ANALYZEDIALOG_H_
#define _ANALYZEDIALOG_H_

#include "ui_AnalyzeDialog.h"

class AnalyzeDialog:public QDialog, protected Ui::AnalyzeDialog {
	Q_OBJECT;
public:
	inline AnalyzeDialog() {setupUi(this);}
};
#endif
