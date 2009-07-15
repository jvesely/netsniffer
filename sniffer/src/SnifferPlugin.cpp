/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "SnifferPlugin.h"
#include "IAnalyzer.h"

Q_EXPORT_PLUGIN2(Sniffer, SnifferPlugin)

bool SnifferPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	return app->registerDeviceList( &devices );
}
