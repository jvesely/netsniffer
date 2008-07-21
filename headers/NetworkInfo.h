#ifndef _NETWORKINFO_H_
#define _NETWORKINFO_H_

#include <QHostAddress>
#include "protocol.h"

struct NetworkInfo{
	QHostAddress sourceIP;
  QHostAddress destinationIP;
  quint16 sourcePort;
  quint16 destinationPort;
  TrProtocol protocol;
};

#endif
