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
	PRINT_DEBUG << "Adding options page" << tab;

	const QString name = tab->name();
	const QIcon icon = tab->icon() ? *tab->icon() : QIcon();
	QWidget* place = new QWidget();
	Q_ASSERT (place);
	const bool success = tab->deploy( place );
	Q_ASSERT (success);

	optionTabs->addTab( place, icon, name );
}
