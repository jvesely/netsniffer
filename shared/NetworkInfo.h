/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "protocol.h"

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
inline bool sameWay( const NetworkInfo& a, const NetworkInfo& b )
{
	return a.protocol == b.protocol
	    && a.destinationIP == b.destinationIP
			&& a.destinationPort == b.destinationPort
			&& a.sourceIP == b.sourceIP
			&& a.sourcePort == b.sourcePort;
}
/*---------------------------------------------------------------------------*/
inline bool backWay( const NetworkInfo& a, const NetworkInfo& b )
{
	return a.protocol == b.protocol
	    && a.destinationIP == b.sourceIP
			&& a.sourceIP == b.destinationIP
			&& a.destinationPort == b.sourcePort
			&& a.sourcePort == b.destinationPort;
}
/*----------------------------------------------------------------------------*/
inline bool operator == ( const NetworkInfo& a, const NetworkInfo& b )
{
	return sameWay( a, b ) || backWay( a, b );
}
