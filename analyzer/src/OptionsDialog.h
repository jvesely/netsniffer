#ifndef _OPTIONSWINDOW_H_
#define _OPTIONSWINDOW_H_

#include "ui_OptionsDialog.h"


class OptionsDialog:protected Ui_OptionsDialog, public QDialog{
	//Q_OBJECT;
public:
	OptionsDialog(QWidget * parent);
		
};

#endif
