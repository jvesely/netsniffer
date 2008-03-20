#include <QByteArray>
#include <QtNetwork/QHostAddress>
enum TrProtocol {
	DUMMY = 0, ICMP = 1, IGMP, GGP, IPiIP, TCP = 6, EGP = 8, UDP = 17, ESP = 50, AH = 51
//http://www.tcpipguide.com/free/t_IPDatagramGeneralFormat.htm
};

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
	enum TrProtocol protocol;
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

