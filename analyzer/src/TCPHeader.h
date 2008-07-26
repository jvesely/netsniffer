#pragma once

class TCPHeader {
	quint16 srcport;
	quint16 destport;
	quint32 seq;
	quint32 acknum;
	quint8 len;
public:
	void parse(QByteArray src);
		
	const quint16 srcPort() const throw();
	const quint16 destPort() const throw();
	const quint32 sequence() const throw();
	const quint8 length() const throw();
	operator QString() const;
};
