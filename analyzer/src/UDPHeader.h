#pragma once

#include "AnalyzerInc.h"

#define UDPLength 8 // UDP header must contain 8 bytes

class UDPHeader {
	quint16 srcport;
	quint16 destport;
	quint16 len;
	quint16 checksum;
public:
	
	void parse(QByteArray src);

	const quint16 srcPort() const throw();
	const quint16 destPort() const throw();
	const quint16 length() const throw();
	operator QString() const;
};
