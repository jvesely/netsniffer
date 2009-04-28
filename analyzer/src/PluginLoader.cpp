#include "PluginLoader.h"
#include "Analyzer.h"
#include "IPlugin.h"

#define DEBUG_TEXT "[ PluginLoader ]:"
#define PRINT_DEBUG qDebug() << DEBUG_TEXT

PluginLoader::PluginLoader( QString file )
:	QPluginLoader( file )
{}
/*----------------------------------------------------------------------------*/
bool PluginLoader::init()
{
	PRINT_DEBUG << "initializing " << fileName();
	QObject * obj = this->instance();
	IPlugin * plugin = qobject_cast<IPlugin *>( obj );

	if (!plugin)
	{
		unload();
		PRINT_DEBUG << "Plugin is not an IPlugin child.";
		return false;
	}
	
	plugin->init( ANALYZER );
	emit statusChanged( loaded() );
	return true;
}
/*----------------------------------------------------------------------------*/
void PluginLoader::load()
{
	this->QPluginLoader::load();
	emit statusChanged( loaded() ); 
}
/*----------------------------------------------------------------------------*/
void PluginLoader::unload()
{
	this->QPluginLoader::unload();
	emit statusChanged( loaded() );
}
