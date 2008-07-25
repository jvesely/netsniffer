#pragma once

#include "protocol.h"

struct NetworkInfo{
	QHostAddress sourceIP;
  QHostAddress destinationIP;
  quint16 sourcePort;
  quint16 destinationPort;
  TrProtocol protocol;
};
