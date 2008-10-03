#pragma once

#include "NetworkInfo.h"

class Packet{
private:
	QByteArray load;
	NetworkInfo info;
	bool last;
	const Packet& operator=(const Packet & other);

public:
	Packet():last(true){};
	bool parse (QByteArray src);
	inline const NetworkInfo networkInfo() const
		{ return info; };
	bool operator==(const Packet & packet) const;
	uint hash() const;
	inline const QByteArray data() const
		{ return load; };
	inline operator QByteArray() const
		{ return load; };
	inline bool isLast() const
		{return last; };
};

uint qHash(const Packet &packet);
