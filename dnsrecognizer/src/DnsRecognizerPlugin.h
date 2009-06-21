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
	DnsRecognizer m_recognizer;
	IAnalyzer* m_app;
	
	Q_OBJECT;
	Q_INTERFACES(IPlugin);
};
