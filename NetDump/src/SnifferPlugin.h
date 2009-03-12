#pragma once

#include "IPlugin.h"
#include "PcapList.h"

class SnifferPlugin: public QObject, public IPlugin
{
	Q_OBJECT;
	Q_INTERFACES(IPlugin);

public:
	bool init();
private:
	PcapList devices;
};
