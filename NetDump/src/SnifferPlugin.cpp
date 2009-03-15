#include "SnifferPlugin.h"
#include "IAnalyzer.h"

Q_EXPORT_PLUGIN2(Sniffer, SnifferPlugin)

bool SnifferPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	return app->registerDeviceList( &devices );
}
