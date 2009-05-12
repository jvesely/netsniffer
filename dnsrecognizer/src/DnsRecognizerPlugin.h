#pragma once

#include "IPlugin.h"
#include "DnsRecognizer.h"

class DnsRecognizerPlugin:public QObject, public IPlugin
{
public:
	bool init( IAnalyzer* app );

private:
	//DnsRecognizer recognizer;
	
	Q_OBJECT;
	Q_INTERFACES(IPlugin);
};
