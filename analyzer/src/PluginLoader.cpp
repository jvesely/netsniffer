/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "PluginLoader.h"
#include "Analyzer.h"
#include "IPlugin.h"

#define DEBUG_TEXT "[ PluginLoader ]:"
#include "debug.h"

/*----------------------------------------------------------------------------*/
bool PluginLoader::init()
{
	PRINT_DEBUG ("initializing " << fileName());
	QObject* obj = instance();
	PRINT_DEBUG ("Instance:" << obj << "Other:" << this->staticInstances());
	IPlugin* plugin = qobject_cast<IPlugin *>( obj );

	if (!plugin)
	{
		unload();
		PRINT_DEBUG ("Plugin is not an IPlugin child.:" << errorString());
		return false;
	}
	PRINT_DEBUG ("Calling init");
	if (!plugin->init( &Analyzer::instance() ))
	{
		unload();
		PRINT_DEBUG ("Plugin init failed.");
		return false;
	}
	PRINT_DEBUG ("Done loading" );
	emit statusChanged( isLoaded() );
	return true;
}
/*----------------------------------------------------------------------------*/
void PluginLoader::load()
{
	QPluginLoader::load();
	emit statusChanged( isLoaded() ); 
}
/*----------------------------------------------------------------------------*/
void PluginLoader::unload()
{
	QPluginLoader::unload();
	emit statusChanged( isLoaded() );
}
