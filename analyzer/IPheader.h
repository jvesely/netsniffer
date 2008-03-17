#include <QByteArray>
#include <QtNetwork/QHostAddress>

class IPheader {
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
	quint8 protocol;
	quint16 checksum;
	bool check;
	QHostAddress source;
	QHostAddress destination;
	quint16 srcport;
	quint16 destport;

	QByteArray bin;
public:
	void parse(QByteArray data);
	operator QString();		
};

