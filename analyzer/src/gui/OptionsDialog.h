/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "ui_OptionsDialog.h"

class IOptionsTab;

class OptionsDialog:public QDialog, private Ui::OptionsDialog
{

	Q_OBJECT;
	
public:
	OptionsDialog( QWidget* parent );

public slots:
		void addOptionsTab( IOptionsTab* tab );
};
