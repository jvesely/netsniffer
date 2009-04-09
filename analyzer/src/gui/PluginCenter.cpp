#include "PluginCenter.h"
#include "Control.h"
#include "uitexts.h"
#include "Analyzer.h"

#define DEBUG_TEXT "[ PluginCenter ]:"
#include "debug.h"

const QString PluginCenter::MYNAME("Plugin Control");

bool PluginCenter::deploy( QWidget * container )
{
	PRINT_DEBUG << "Deploying";
	setupUi( container );
	connect( pushButtonAdd, SIGNAL(clicked()), this, SLOT(addPlugin()) );
	connect( pushButtonRemoveAll, SIGNAL(clicked()), this, SLOT(removeAll()) );

	DropArea * controlArea = new DropArea();

	Q_ASSERT (controlArea);
	Q_ASSERT (m_areas.empty());  // for now we only support one DropArea
	m_areas.append( controlArea );
	PRINT_DEBUG << "Area added:" << controlArea ;

	connect( controlArea, SIGNAL(destroyed( QObject * )), this, SLOT(removeArea( QObject * )) );
	connect( controlArea, SIGNAL(newPlugin( const QString )), this, SIGNAL( newPlugin( const QString )) );

	Q_ASSERT (verticalLayout);
	verticalLayout->insertWidget( 0, controlArea );

	PluginList current = ANALYZER->currentPlugins();
	PRINT_DEBUG << "Plugins:" << current;

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
		NULL, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ) );
	if (QFile::exists( file ))
		emit newPlugin( file );
}
/*----------------------------------------------------------------------------*/
void PluginCenter::removeAll()
{
	const int ret = QMessageBox::warning(
		NULL, tr( UI_DELETE_ALL ), tr( UI_DELETE_ALL_EXT ), QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::Yes)
	{
		emit deleteAll();
	}
}
/*----------------------------------------------------------------------------*/
void PluginCenter::addPluginControl( PluginLoader * plugin )
{
	//if (!controlArea) return; //not deployed yet
	for (AreaList::Iterator it = m_areas.begin(); it != m_areas.end(); ++it)
	{
		Q_ASSERT (*it);
		Control * control = new Control( *it, plugin );
		Q_ASSERT ((*it)->layout());
		(*it)->layout()->addWidget( control );
	}

	connect( this, SIGNAL(deleteAll()), plugin, SLOT(deleteLater()) );
}
/*----------------------------------------------------------------------------*/
void PluginCenter::removeArea( QObject* object )
{
	const int res = m_areas.removeAll( (DropArea*) object );
	Q_ASSERT( res == 1 );
	PRINT_DEBUG << "Area removed:" << object;
}
/*
void PluginCenter::dropEvent(QDropEvent* event)
{
	const QString path = (event->mimeData()->text()).remove("file://").trimmed();
	if (QFile::exists(path))
		emit newPlugin( path );
}
void PluginCenter::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasText())
		event->accept();
}
*/
