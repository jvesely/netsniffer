#include <QObject>
#include <QByteArray>
#include "cPacket.h"
#include "IDevice.h"

class cAnalyzer:public QObject{
	Q_OBJECT
public slots:
	void analyze(IDevice * dev, QByteArray * data);
};