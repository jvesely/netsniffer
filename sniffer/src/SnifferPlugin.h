#pragma once

#include "IPlugin.h"
#include "PcapList.h"


class SnifferPlugin:public QObject, public IPlugin
{
public:
	bool init( IAnalyzer* app );

private:
	PcapList devices;

	Q_OBJECT;
	Q_INTERFACES(IPlugin);
};
