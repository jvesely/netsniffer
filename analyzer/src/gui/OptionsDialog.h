#pragma once

#include "ui_OptionsDialog.h"

class IOptionsTab;

class OptionsDialog:public QDialog, protected Ui_OptionsDialog
{

	Q_OBJECT;
	
public:
	OptionsDialog( QWidget* parent );

public slots:
		void addOptionsTab( IOptionsTab* tab );
};
