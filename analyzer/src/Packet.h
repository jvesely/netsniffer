#pragma once

#include "NetworkInfo.h"

class Packet
{
public:
	Packet():m_last( true ) {};
	Packet( const QByteArray& src );
	bool parse ( const QByteArray& src, QString& error ) throw ();
	inline const NetworkInfo& networkInfo() const throw()
		{ return m_info; };
	inline const QByteArray data() const throw()
		{ return m_load; };
	inline operator QByteArray() const throw()
		{ return data(); };
	inline bool isLast() const throw()
		{ return m_last; };

private:
	QByteArray m_load;
	NetworkInfo m_info;
	bool m_last;
	
	Q_DISABLE_COPY (Packet);
};
