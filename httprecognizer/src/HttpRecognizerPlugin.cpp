#include "HttpRecognizerPlugin.h"

Q_EXPORT_PLUGIN2(HttpRecognizer, HttpRecognizerPlugin)

bool HttpRecognizerPlugin::init( IAnalyzer* app )
{
	Q_ASSERT (app);
	mApp = app;
	return mApp->registerRecognizer( &mRecognizer );
}
/*---------------------------------------------------------------------------*/
HttpRecognizerPlugin::~HttpRecognizerPlugin()
{
	Q_ASSERT (mApp);
	mApp->unregisterRecognizer( &mRecognizer );
}
