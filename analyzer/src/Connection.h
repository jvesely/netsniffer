#pragma once

#include "Packet.h"
#include "IConnection.h"

#define DEFAULT_MAX 50


typedef QPair<bool, QByteArray> DirectedData;
typedef QList<DirectedData> DirectedDataList;

class Connection:public QObject, public IConnection
{
public slots:
	void setAutoPurge( bool on );
	virtual void close();
	void die();
	virtual void update();

signals:
	void packetArrived( Connection* me );
	void statusChanged( Connection* me );
	void finished( Connection* me );

public:
	~Connection();

	Connection( const Packet& packet );
	inline Connection& operator << ( const Packet& packet )
		{ return addPacket( packet ),*this; }

	inline const NetworkInfo& networkInfo() const 
		{ return m_info; };

	inline ConnectionStatus status() const
		{ QReadLocker lock(&m_guard); return m_status; };

	inline const QByteArray getLastPacketForward() const 
		{ QReadLocker lock(&m_guard); return m_lastPacketForward; };

	inline const QByteArray getLastPacketBack() const 
		{ QReadLocker lock(&m_guard); return m_lastPacketBack; };

	inline int speedForward() const
		{ QReadLocker lock(&m_guard); return m_speedUp; };	

	inline int speedBack() const
		{ QReadLocker lock(&m_guard); return m_speedDown; };

	inline int packetCountForward() const
		{ QReadLocker lock(&m_guard); return m_countForward; };

	inline int packetCountBack() const
		{ QReadLocker lock(&m_guard); return m_countBack; };

	virtual bool addPacket( const Packet& packet );

protected:
	uint m_timeout;
	ConnectionStatus m_status;

private:
	const NetworkInfo m_info;
	
	DirectedDataList m_data;
	
	QByteArray m_lastPacketForward;
	QByteArray m_lastPacketBack;
	
	uint m_countForward;
	uint m_countBack;
	bool m_killDead;
	int m_speedUp;
	int	m_speedDown;
	int m_dataUp;
	int m_dataDown;

	mutable QReadWriteLock m_guard;

	Q_DISABLE_COPY (Connection);
	Q_OBJECT;
};
