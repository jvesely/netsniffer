#include "OptionsDialog.h"
#include "Control.h"
#include "Analyzer.h"

#include "uitexts.h"

#define DEBUG_TEXT "[ OptionsDialog ]: "
#include "debug.h"

OptionsDialog::OptionsDialog( QWidget* parent ): QDialog( parent )
{
	setupUi( this );
	connect( buttonBox, SIGNAL(accepted()), ANALYZER, SLOT(saveSettings()) );
	optionTabs->clear();
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addOptionsPage( IOptionsPage* new_page )
{
	Q_ASSERT (new_page);
	PRINT_DEBUG << "Adding options page" << new_page;

	const QString name = new_page->name();
	const QIcon icon = new_page->icon() ? *new_page->icon() : QIcon();
	QWidget* place = new QWidget();
	const bool success = new_page->deploy( place );
	Q_ASSERT (success);

	optionTabs->addTab( place, icon, name );
}
