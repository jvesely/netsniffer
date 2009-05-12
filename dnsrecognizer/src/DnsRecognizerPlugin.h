#pragma once

#include "IPlugin.h"
#include "DnsRecognizer.h"

class DnsRecognizerPlugin:public QObject, public IPlugin
{
public:
	bool init( IAnalyzer* app );
	~DnsRecognizerPlugin();

private:
	DnsRecognizer m_recognizer;
	IAnalyzer* m_app;
	
	Q_OBJECT;
	Q_INTERFACES(IPlugin);
};
