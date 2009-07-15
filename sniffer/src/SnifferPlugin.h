/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
