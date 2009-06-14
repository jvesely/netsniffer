#pragma once

#include "NetworkInfo.h"
#include "IRecognizer.h"

class IConnection: public QObject
{
public:
	enum ConnectionStatus {	Alive, Dead, TimedOut, Closed	};
	enum Direction { Forward, Back };
	typedef QPair<Direction, QByteArray> DirectedPacket;

	virtual ~IConnection() {};
	virtual void close() = 0;
	virtual const NetworkInfo& networkInfo() const = 0;
	virtual const DirectedPacket nextPacket() = 0;
	virtual const DirectedPacket topPacket() const = 0;
	virtual uint waitingPackets() = 0;

	virtual int packetCountForward() const = 0;
	virtual int packetCountBack() const = 0;
	virtual ConnectionStatus status() const = 0;

	inline const QVariant comment( const QVariant& no_comment = "Not Recognized" )
		{ return m_recognizer ? m_recognizer->comment( this ) : no_comment; }

	inline bool showDetails()
		{ return m_recognizer ? m_recognizer->showDetails( this ) : false; }

	inline void setRecognizer( IRecognizer* recognizer )
		{ m_recognizer = recognizer; }
	
	inline IRecognizer* recognizer()
		{ return m_recognizer; }

protected:
	QPointer<IRecognizer> m_recognizer;
};
