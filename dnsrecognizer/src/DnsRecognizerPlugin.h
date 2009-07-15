#pragma once

#include "IAnalyzer.h"
#include "IPlugin.h"
#include "DnsRecognizer.h"

class DnsRecognizerPlugin: public QObject, public IPlugin
{
public:
	bool init( IAnalyzer* app );
	~DnsRecognizerPlugin();

private:
	DnsRecognizer mRecognizer;
	IAnalyzer* mApp;
	
	Q_OBJECT;
	Q_INTERFACES(IPlugin);
};
