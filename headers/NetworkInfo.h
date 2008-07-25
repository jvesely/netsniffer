#pragma once

#include "qtInc.h"

#include "protocol.h"

struct NetworkInfo{
	QHostAddress sourceIP;
  QHostAddress destinationIP;
  quint16 sourcePort;
  quint16 destinationPort;
  TrProtocol protocol;
};
