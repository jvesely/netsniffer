#pragma once

class IAnalyzer;

class IPlugin
{
public:
	virtual bool init( IAnalyzer* app ) = 0;
	virtual ~IPlugin() {};
};

Q_DECLARE_INTERFACE(IPlugin, "Generic Plugin Interface/1.0");
