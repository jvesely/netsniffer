#include <QByteArray>
#include <QtNetwork/QHostAddress>
#include "protocol.h"

class IPHeader {
private:
	quint8 version;
	quint8 length;
	quint8 type;
	quint16 totlen;
	quint16 id;
	bool fragment;
	bool morefrags;
	quint16 froffset;
	
	quint8 ttl;
	TrProtocol protocol;
	quint16 checksum;
	bool check;
	QHostAddress source;
	QHostAddress destination;

	QByteArray bin;
public:
	void parse(QByteArray data);
	const QHostAddress srcAddress() const throw();
	const QHostAddress destAddress() const throw();
	const TrProtocol trProtocol() const throw();
	const int headerLength() const throw();
	const int packetLength() const throw();

	operator QString() const;		
};

