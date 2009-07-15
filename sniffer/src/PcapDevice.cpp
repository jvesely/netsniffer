#include "PcapDevice.h"
#include "Ethernet.h"

#ifndef QT_NO_DEBUG
#define DEBUG_TEXT "[ PcapDevice ]: "
#define PRINT_DEBUG(arg) qDebug() << DEBUG_TEXT << arg
#else
#define PRINT_DEBUG(arg)
#endif

/*----------------------------------------------------------------------------*/
namespace Pcap
{
	struct DLT_LINUX_SSL_HEADER
	{
		quint16 type;
		quint16 arphdr;
		quint16 addrlen;
		quint8 linkheader[8];
		quint16 ethertype;
	};
	static const int LOOPBACK_HEADER_SIZE = 4;

	void handlePacket( u_char* ptr, const struct pcap_pkthdr* header, const u_char* packet )
	{
		PcapDevice* device = (PcapDevice*) ptr;
		Q_ASSERT (device);
		Q_ASSERT (header);
		device->packet( QByteArray::fromRawData( (char*)packet, header->len ) );
	}
}
/*----------------------------------------------------------------------------*/
static const int READ_TIMEOUT = 100; //ms
static const int SNAP_LENGTH = 0x10000;
static const int INFINITE_COUNT = -1;
/*----------------------------------------------------------------------------*/
QString PcapDevice::translateName( const QString& name )
{
#ifdef Q_OS_WIN32
	static const QString prefix = "\\Device\\NPF_";
	static const QString registryKey = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Network\\"
"{4D36E972-E325-11CE-BFC1-08002BE10318}";

	if (name.startsWith( prefix ))
	{
		const QSettings registry( QString( "%1\\%2\\Connection" ).arg(
			registryKey, name.mid( prefix.length() ) ), QSettings::NativeFormat );
		return registry.value( "Name", name ).toString();
	}
#endif

	return name;
}
/*----------------------------------------------------------------------------*/
PcapDevice::PcapDevice( pcap_if_t *dev )
: mHandle( 0 ), mType( 0 ),
	mPcapName( dev->name ), mName( translateName( dev->name ) ),
	mDesc( dev->description )
{}
/*----------------------------------------------------------------------------*/
PcapDevice::~PcapDevice()
{
	PRINT_DEBUG ("Device Dying");
	if (isRunning())
		captureStop();
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStart()
{
	if (!open())
		return false;
	PRINT_DEBUG ("Starting...");
	start();
	setPriority( HighestPriority );
	return isRunning();
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStop()
{
	Q_ASSERT (mHandle);
	pcap_breakloop( mHandle );
	if (!wait( 250 ))
	{
		terminate();
		wait();
		close();
	}
	return isRunning();
}
/*----------------------------------------------------------------------------*/
pcap_t* PcapDevice::open()
{
	PRINT_DEBUG ("Opening Device...");
	char err[PCAP_ERRBUF_SIZE];

	if (mHandle)
		return NULL;

	const int promisc = (mPcapName == "any") ? 0 : 1; // ok serious bug here device any not working promisc, leads to crash in glibc
	mHandle = pcap_open_live( mPcapName.toAscii().data(), SNAP_LENGTH, promisc, READ_TIMEOUT, err );	

	if (mHandle)
	{
		mType = pcap_datalink( mHandle );
		PRINT_DEBUG ("Datalink type:" << pcap_datalink_val_to_name( mType ));
	} else {
		mError = err;
		PRINT_DEBUG ("ERROR:" << err);
	}

	return mHandle;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::close()
{
	PRINT_DEBUG ("Closing device");
	Q_ASSERT (mHandle);

	pcap_close( mHandle );
	emit captureStopped( this );
	mHandle = 0;
	mType = 0;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::run()
{
	emit captureStarted( this );

	const int ret = pcap_loop( mHandle, INFINITE_COUNT, &Pcap::handlePacket, (u_char*)this );
	close();
}
/*----------------------------------------------------------------------------*/
void PcapDevice::packet( const QByteArray packet )
{
	QByteArray load = link2IP( packet );
	if (!load.isNull())
		emit packetArrived( this, load );
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::link2IP( const QByteArray data )
{
	switch (mType)
	{
		case DLT_LOOP:
		case DLT_NULL: //BSD loopback
			return data.mid( Pcap::LOOPBACK_HEADER_SIZE );
			//QByteArray( data, len ).remove( 0, Pcap::LOOPBACK_HEADER_SIZE );
		case DLT_EN10MB: //Ethernet
			return parseEthernet( data );
		case DLT_IEEE802_11_RADIO:
			PRINT_DEBUG ("802.11");
			return QByteArray();
		case DLT_LINUX_SLL:
			return parseSll( data );
		case DLT_RAW: //pcap raw ip
			PRINT_DEBUG ("DLT_RAW_IP");
			return data;
		default: //Other types
			PRINT_DEBUG ("UNKNOWN datalink type: " << pcap_datalink_val_to_name( mType ));
			return QByteArray();
	}
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::parseSll( const QByteArray data )
{
	Pcap::DLT_LINUX_SSL_HEADER* header =
		(Pcap::DLT_LINUX_SSL_HEADER*) data.data();
	if (qFromBigEndian( header->ethertype ) == Ethernet::EthernetII::IP ) {
		return data.mid( sizeof( Pcap::DLT_LINUX_SSL_HEADER ) );
	}
	
	return QByteArray();
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::parseEthernet( const QByteArray data )
{
	using namespace Ethernet;

	Q_ASSERT (sizeof( HEADER_802_3 ) == 14);
	const int size_or_type = qFromBigEndian( ((HEADER_802_3*)data.data())->size );
	if (size_or_type > MAXIMUM_SIZE) //EthernetII
	{
		if (size_or_type == EthernetII::IP) 
			return data.mid( sizeof( HEADER_802_3 ) );

		return QByteArray();
	}

	Q_ASSERT (sizeof( SAP_HEADER ) == 3 );
	const SAP_HEADER* header = (SAP_HEADER*)(data.data() + sizeof( HEADER_802_3 ));
	switch (header->dsap)
	{
		case RAW: //0xff
			break; // can only carry IPX packets
		case SNAP:// 0xaa
		{
			Q_ASSERT (sizeof( SNAP_HEADER ) == 8);
			/*
			 * Linux doesn't handle IP in SNAP and the bridge netfilter code also doesn't.
			 */
			Q_ASSERT (header->ssap == SNAP); //ssap should be 0xAA too
			const SNAP_HEADER* snap_header = (SNAP_HEADER*) header;
			PRINT_DEBUG ("Ethernet SNAP");
			if (qFromBigEndian( snap_header->ethertype ) == EthernetII::IP)
				return data.mid( sizeof( HEADER_802_3 ) + sizeof( SNAP_HEADER ) );
			break;
		}

		case SAP_IP://802.2 + 802.3
			/* Cannot be done as there is no SAP assigned to ARP protocol */
			PRINT_DEBUG ("802.2/802.3");
			return data.mid( sizeof( HEADER_802_3 ) + sizeof( SAP_HEADER ) );
	}
	return QByteArray();
}
