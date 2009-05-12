#include "DnsRecognizerPlugin.h"
#include "IAnalyzer.h"

Q_EXPORT_PLUGIN2(DnsRecognizer, DnsRecognizerPlugin)

bool DnsRecognizerPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	return false;
//	return app->registerRecognizer( &recognizer );
}
