#include "DnsRecognizerPlugin.h"

Q_EXPORT_PLUGIN2(DnsRecognizer, DnsRecognizerPlugin)

bool DnsRecognizerPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	m_app = app;
	m_recognizer.setDnsCache( app->dnsCache() );
	return app->registerRecognizer( &m_recognizer );
}
/*----------------------------------------------------------------------------*/
DnsRecognizerPlugin::~DnsRecognizerPlugin()
{
	Q_ASSERT(m_app);
	m_app->unregisterRecognizer( &m_recognizer );
}
