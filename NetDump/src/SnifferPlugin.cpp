#include "SnifferPlugin.h"
//#include "IAnalyzer.h"

Q_EXPORT_PLUGIN2(Sniffer, SnifferPlugin)

bool SnifferPlugin::init()
{
	//IAnalyzer::instance()->registerDeviceList( &devices );
	return true;
}
