#include "OptionsDialog.h"
#include "Control.h"
#include "Analyzer.h"

#include "uitexts.h"

OptionsDialog::OptionsDialog(QWidget * parent): QDialog( parent )
{
	setupUi( this );
	connect( pushButtonAdd, SIGNAL(clicked()), this, SLOT(addPlugin()) );
	connect( pushButtonDiscard, SIGNAL(clicked()), this, SLOT(discard()) );
	connect( buttonBox, SIGNAL(accepted()), ANALYZER, SLOT(saveSettings()) );
};
/*----------------------------------------------------------------------------*/
void OptionsDialog::addPlugin()
{
	QString module = QFileDialog::getOpenFileName(
		this, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ));
//	qDebug() << module << QFile::exists(module);
	if ( QFile::exists(module) )
		emit newPlugin(module);
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::discard()
{
	const int ret = QMessageBox::warning(
		this, tr( UI_DELETE_ALL ), tr( UI_DELETE_ALL_EXT ), QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::Yes)
		emit discardPlugins();
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::addControl( QPluginLoader * plugin )
{
	qDebug() << "Adding rercognizer control";
	
	Control * newCtrl = new Control( this, plugin );
	connect( this, SIGNAL(discardPlugins()), plugin, SLOT(deleteLater()) );

	QVBoxLayout * layout = (QVBoxLayout *)scrollAreaWidgetContents->layout();
	layout->insertWidget( layout->count() - 1, newCtrl ); // add right before the spacer
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::dropEvent(QDropEvent * event)
{
	QString path = (event->mimeData()->text()).remove("file://").trimmed();
	if ( QFile::exists(path))
		emit newPlugin(path);
}
/*----------------------------------------------------------------------------*/
void OptionsDialog::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasText())
		event->accept();
}
