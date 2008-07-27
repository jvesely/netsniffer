#pragma once

#include "NetworkInfo.h"

class Packet{
private:
	QByteArray load;
	NetworkInfo info;
	const Packet& operator=(const Packet & other);

public:
	Packet(){};
	bool parse (QByteArray src);
	inline const NetworkInfo networkInfo() const
		{ return info; };
	bool operator==(const Packet & packet) const;
	uint hash() const;
	inline const QByteArray data() const
		{ return load; };
	inline operator QByteArray()  
		{ return load; };
};

uint qHash(const Packet &packet);
