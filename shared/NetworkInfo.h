#pragma once

#include "protocol.h"

#define SAME_WAY(a,b) \
	( a.protocol == b.protocol && \
	a.sourceIP == b.sourceIP && \
	a.destinationIP == b.destinationIP && \
	a.sourcePort == b.sourcePort && \
	a.destinationPort == b.destinationPort )

#define BACK_WAY(a,b) \
  ( a.protocol == b.protocol && \
  a.sourceIP == b.destinationIP && \
  a.destinationIP == b.sourceIP && \
  a.sourcePort == b.destinationPort && \
  a.destinationPort == b.sourcePort )


struct NetworkInfo
{
	QHostAddress sourceIP;
  QHostAddress destinationIP;
  quint16 sourcePort;
  quint16 destinationPort;
  TrProtocol protocol;
};
/*----------------------------------------------------------------------------*/
inline int qHash( const NetworkInfo& info )
{
	return qHash( info.sourceIP.toIPv4Address() ) ^
		qHash( info.destinationIP.toIPv4Address() ) ^
		qHash( info.sourcePort ) ^
		qHash( info.destinationPort ) ^
		qHash( (int)info.protocol );
}
/*----------------------------------------------------------------------------*/
inline bool operator == ( const NetworkInfo& a, const NetworkInfo& b)
{
	return SAME_WAY(a,b) || BACK_WAY(a,b);
}

#undef SAME_WAY
#undef BACK_WAY
