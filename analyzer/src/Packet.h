/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "NetworkInfo.h"

class Packet
{
public:
	Packet( const NetworkInfo& info, const QByteArray& data, bool last = false )
	: mInfo( info ), mLoad( data ), mLast( last )	{};
	
	static Packet parse( const QByteArray& src );

	inline const NetworkInfo& networkInfo() const throw()
		{ return mInfo; };
	inline const QByteArray data() const throw()
		{ return mLoad; };
	inline operator QByteArray() const throw()
		{ return data(); };
	inline bool isLast() const throw()
		{ return mLast; };

private:
	const NetworkInfo mInfo;
	const QByteArray mLoad;
	const bool mLast;
};
