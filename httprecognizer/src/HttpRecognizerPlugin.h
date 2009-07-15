/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IAnalyzer.h"
#include "IPlugin.h"
#include "HttpRecognizer.h"

class HttpRecognizerPlugin: public QObject, public IPlugin
{
public:
	bool init( IAnalyzer* app );
	~HttpRecognizerPlugin();

private:
	HttpRecognizer mRecognizer;
	IAnalyzer* mApp;

	Q_OBJECT;
	Q_INTERFACES (IPlugin);
};
