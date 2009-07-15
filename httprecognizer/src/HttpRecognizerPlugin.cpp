/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
