#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include "CPacket.h"

class CConnection;

class CConnection:public QObject {
	Q_OBJECT;
	QHostAddress addrSrc;
	QHostAddress addrDest;
	quint16 portSrc;
	quint16 portDest;
	TrProtocol protocol;
	QByteArray dataForw;
	QByteArray dataBack;
	QTimer fuse;
	int timeout;
	uint countFr;
	uint countBc;

	void reset();
public slots:
	void timedout();
//	void addPacket(const CPacket& packet);
signals:
	void packetProc();
	
public:
	CConnection();
	CConnection(const CConnection& connection);
	CConnection & operator<<(const CPacket& packet);
	operator QByteArray() const;
	const QString toString() const;
};
