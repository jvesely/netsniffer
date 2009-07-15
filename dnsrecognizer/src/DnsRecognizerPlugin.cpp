/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "DnsRecognizerPlugin.h"

Q_EXPORT_PLUGIN2(DnsRecognizer, DnsRecognizerPlugin)

bool DnsRecognizerPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	mApp = app;
	mRecognizer.setDnsCache( app->dnsCache() );
	return app->registerRecognizer( &mRecognizer );
}
/*----------------------------------------------------------------------------*/
DnsRecognizerPlugin::~DnsRecognizerPlugin()
{
	Q_ASSERT(mApp);
	mApp->unregisterRecognizer( &mRecognizer );
}
