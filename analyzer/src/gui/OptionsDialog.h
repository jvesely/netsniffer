#pragma once

#include "ui_OptionsDialog.h"

class IOptionsPage;

class OptionsDialog:public QDialog, protected Ui_OptionsDialog
{

	Q_OBJECT;
	
public:
	OptionsDialog( QWidget* parent );

public slots:
		void addOptionsPage( IOptionsPage* new_page );
};
