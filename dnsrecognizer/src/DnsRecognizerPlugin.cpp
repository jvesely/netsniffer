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
