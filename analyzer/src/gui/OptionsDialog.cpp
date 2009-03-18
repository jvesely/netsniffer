#include "OptionsDialog.h"
#include "Control.h"
#include "Analyzer.h"

#include "uitexts.h"
#define DEBUG_TEXT "[ OptionsDialog ]: "

OptionsDialog::OptionsDialog(QWidget * parent): QDialog( parent )
{
	setupUi( this );
	connect( buttonBox, SIGNAL(accepted()), ANALYZER, SLOT(saveSettings()) );
	optionTabs->clear();
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addOptionsPage( IOptionsPage* new_page )
{
	qDebug() << DEBUG_TEXT << "Adding options page" << new_page;
	Q_ASSERT (new_page);

	const QString name = new_page->name();
	const QIcon icon = new_page->icon() ? *new_page->icon() : QIcon();
	QWidget* place = new QWidget();
	const bool success = new_page->deploy( place );
	Q_ASSERT (success);

	optionTabs->addTab( place, icon, name );
//	new_page.second->setParent( NULL );
}
/*
void OptionsDialog::addPlugin()
{
	QString file = QFileDialog::getOpenFileName(
		this, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ));
//	qDebug() << module << QFile::exists(module);
	if ( QFile::exists( file ) )
		emit newPlugin( file );
}
void OptionsDialog::discard()
{
	
	const int ret = QMessageBox::warning(
		this, tr( UI_DELETE_ALL ), tr( UI_DELETE_ALL_EXT ), QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::Yes)
		emit discardPlugins();
}
void OptionsDialog::addControl( QPluginLoader * plugin )
{
	qDebug() << "Adding plugin control";
	
	Control * newCtrl = new Control( this, plugin );
	connect( this, SIGNAL(discardPlugins()), plugin, SLOT(deleteLater()) );

	QVBoxLayout * layout = (QVBoxLayout *)scrollAreaWidgetContents->layout();
	layout->insertWidget( layout->count() - 1, newCtrl ); // add right before the spacer
}
void OptionsDialog::dropEvent(QDropEvent * event)
{
	QString path = (event->mimeData()->text()).remove("file://").trimmed();
	if ( QFile::exists(path))
		emit newPlugin(path);
}
void OptionsDialog::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasText())
		event->accept();
}
*/
