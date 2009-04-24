#include "SorterPool.h"
#include "PacketSorter.h"

#define DEBUG_TEXT "[ SorterPool ]:"
#include "debug.h"

#define THREAD_LIMIT 10

/*----------------------------------------------------------------------------*/
void SorterPool::addPacket( QByteArray data ) throw()
{
	m_waitingPackets.enqueue( data );
}
/*----------------------------------------------------------------------------*/
void SorterPool::addSorters( uint n ) throw()
{
	if (n > THREAD_LIMIT) return;

	for(;n;--n)
	{
		PRINT_DEBUG << "Adding sorting thread.";
		PacketSorter * sorter = 
			new PacketSorter( &m_waitingPackets, &m_existingConnections );
		Q_ASSERT (sorter);

		connect( sorter, SIGNAL(connection(Connection*)), this, SIGNAL(connection(Connection *)), Qt::DirectConnection );
		m_availableSorters.append( sorter );
		sorter->start();
	}
}
/*----------------------------------------------------------------------------*/
void SorterPool::removeSorters( uint n ) throw()
{
	if ( n > m_availableSorters.count()) return;
	for (;n;--n){
		PRINT_DEBUG << "Removing sorter from pool of " << m_availableSorters.count();
		PacketSorter* sorter = m_availableSorters.takeFirst();
		Q_ASSERT (sorter);
		sorter->stop(); 
	}
}
/*----------------------------------------------------------------------------*/
SorterPool::~SorterPool() throw()
{
	PRINT_DEBUG << "Killing sorting workers..." << m_availableSorters.count();
	
	while (!m_availableSorters.isEmpty()) {
		m_availableSorters.takeFirst()->stop();
	}
	
	// ok no more sorters in my list
	m_waitingPackets.release(); // just to make sure threads will wake up
}
