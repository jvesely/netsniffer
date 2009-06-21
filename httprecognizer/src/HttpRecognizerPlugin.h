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
