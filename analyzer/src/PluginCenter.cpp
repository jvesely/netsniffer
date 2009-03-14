#include "PluginCenter.h"
#include "Control.h"
#include "uitexts.h"
#include "Analyzer.h"

const QString PluginCenter::MYNAME("Plugin Control");

bool PluginCenter::deploy( QWidget * container )
{
	setupUi( container );
	connect( pushButtonAdd, SIGNAL(clicked()), this, SLOT(addPlugin()) );
	connect( pushButtonRemoveAll, SIGNAL(clicked()), this, SLOT(removeAll()) );

	controlArea->layout()->setAlignment( Qt::AlignTop );
//	controlArea->setAcceptDrops( true );

	PluginList current = ANALYZER->currentPlugins();
	for (PluginList::iterator it = current.begin(); it != current.end(); ++it)
	{
		addPluginControl( *it );
	}
	return true;
};
/*----------------------------------------------------------------------------*/
void PluginCenter::addPlugin()
{
	QString file = QFileDialog::getOpenFileName(
		controlArea, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ) );
	if (QFile::exists( file ))
		emit newPlugin( file );
}
/*----------------------------------------------------------------------------*/
void PluginCenter::removeAll()
{
	const int ret = QMessageBox::warning(
		controlArea, tr( UI_DELETE_ALL ), tr( UI_DELETE_ALL_EXT ), QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::Yes)
	{
		emit deleteAll();
	}
}
/*----------------------------------------------------------------------------*/
void PluginCenter::addPluginControl( QPluginLoader * plugin )
{
	Control * control = new Control( controlArea, plugin );
	
	controlArea->layout()->addWidget( control );

	connect( this, SIGNAL(deleteAll()), plugin, SLOT(deleteLater()) );
}
/*----------------------------------------------------------------------------*/
void PluginCenter::dropEvent(QDropEvent* event)
{
	const QString path = (event->mimeData()->text()).remove("file://").trimmed();
	if (QFile::exists(path))
		emit newPlugin( path );
}
/*----------------------------------------------------------------------------*/
void PluginCenter::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasText())
		event->accept();
}
