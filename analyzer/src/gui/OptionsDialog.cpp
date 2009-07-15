/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "OptionsDialog.h"
#include "IOptionsTab.h"

#include "uitexts.h"

#define DEBUG_TEXT "[ OptionsDialog ]: "
#include "debug.h"

OptionsDialog::OptionsDialog( QWidget* parent ): QDialog( parent )
{
	setupUi( this );
	optionTabs->clear();
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addOptionsTab( IOptionsTab* tab )
{
	Q_ASSERT (tab);
	PRINT_DEBUG ("Adding options page" << tab);

	const QString name = tab->name();
	const QIcon icon = tab->icon() ? *tab->icon() : QIcon();

	QWidget* place = new QWidget();
	Q_ASSERT (place);

	const bool success = tab->deploy( place );

	Q_UNUSED (success);
	Q_ASSERT (success);

	optionTabs->addTab( place, icon, name );
}
