#include "PcapDevice.h"

#define DEBUG_TEXT "[ Pcap Device ]: "
#define PRINT_DEBUG qDebug() << DEBUG_TEXT
/*----------------------------------------------------------------------------*/
namespace Ethernet
{
	namespace EthernetII
	{
		enum EthernetIITypes
		{
			IP = 0x0800,
			IPv6 = 0x86DD
		};
	};
	
	struct HEADER_802_3
	{
		quint8 sourceMAC[6];
		quint8 destinationMAC[6];
		quint16 size;
	};

	struct SAP_HEADER
	{
		quint8 dsap;
		quint8 ssap;
		quint8 control;
	};

	struct SNAP_HEADER
	{
		quint8 dsap;
		quint8 ssap;
		quint8 control;
		quint8 orgcode[3];
		quint16 ethertype;
	};

	static const int MAXIMUM_SIZE = 1500;
	static const int RAW = 0xFF;
	static const int SNAP = 0xAA;
	//static const int HEADER_SIZE = 14; // 2 * 6b (MAC) + 2b (type)
	static const int SAP_IP = 0x06;
}
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
}
/*----------------------------------------------------------------------------*/
static const int READ_TIMEOUT = 100; //ms
static const int SNAP_LENGTH = 0x10000;
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
: mHandle( 0 ), mPcapName( dev->name ), mName( translateName( dev->name ) ), 
	mDesc( dev->description ), mType( 0 ), mCapturing( false )
{}
/*----------------------------------------------------------------------------*/
PcapDevice::~PcapDevice()
{
	PRINT_DEBUG << "Device Dying";
	if (mCapturing)
		captureStop();
	else
		close();
}
/*----------------------------------------------------------------------------*/
pcap_t* PcapDevice::open()
{
	PRINT_DEBUG << "Opening Device...";
	char* err = 0;
	if (!mHandle)
	{
		const int promisc = (mPcapName == "any") ? 0 : 1; // ok serious bug here device any not working promisc, leads to crash in glibc
		mHandle = pcap_open_live( mPcapName.toAscii().data(), SNAP_LENGTH, promisc, READ_TIMEOUT, err );
		
	}
	
	if (mHandle)
	{
		mType = pcap_datalink( mHandle );
		PRINT_DEBUG << "Datalink type:" << pcap_datalink_val_to_name( mType );
		return mHandle;
	}
	
	PRINT_DEBUG << "ERROR:" << err;
	return 0;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::close()
{
	if (mHandle)
		pcap_close( mHandle );
	mHandle = 0;
	mType = 0;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::run()
{
	pcap_pkthdr header;
	const u_char* data;
	mCapturing = true;
	emit captureStarted( this );
	while (mCapturing)
	{
		if ((data = pcap_next( mHandle, &header )))
			packet( header, data );
	}
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStart()
{
	if (mCapturing || !open())
		return false;
	PRINT_DEBUG << "Starting...";
	start();
	return true;
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStop()
{
	mCapturing = false;
	terminate();
	wait();
	close();
	emit captureStopped( this );
	return !mCapturing;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::packet( pcap_pkthdr header, const u_char* data )
{
	QByteArray load = link2IP( (char*)data, header.len );
	if (!load.isNull())
		emit packetArrived( this, load );
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::link2IP( const char* data, int len )
{
	switch (mType)
	{
		case DLT_LOOP:
		case DLT_NULL: //BSD loopback
			return QByteArray( data, len ).remove( 0, Pcap::LOOPBACK_HEADER_SIZE );
		case DLT_EN10MB: //Ethernet
			return parseEthernet( QByteArray::fromRawData( data, len ) );
		case DLT_IEEE802_11_RADIO:
			PRINT_DEBUG << "802.11";
			return QByteArray();
		case DLT_LINUX_SLL:
			return parseSll( QByteArray::fromRawData( data, len ) );
		case DLT_RAW: //pcap raw ip
			PRINT_DEBUG << "DLT_RAW_IP";
			return QByteArray::fromRawData( data, len );
		default: //Other types
			PRINT_DEBUG << "UNKNOWN datalink type: " << pcap_datalink_val_to_name( mType );
			return QByteArray();
	}
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::parseSll( QByteArray data )
{
	Pcap::DLT_LINUX_SSL_HEADER* header =
		(Pcap::DLT_LINUX_SSL_HEADER*) data.data();
	if (qFromBigEndian( header->ethertype ) == Ethernet::EthernetII::IP ) {
		return data.remove( 0, sizeof( Pcap::DLT_LINUX_SSL_HEADER ) );
	}
	
	return QByteArray();
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::parseEthernet( QByteArray data )
{
	using namespace Ethernet;

	Q_ASSERT (sizeof( HEADER_802_3 ) == 14);
	const int size_or_type = qFromBigEndian( ((HEADER_802_3*)data.data())->size );
	if (size_or_type > MAXIMUM_SIZE) //EthernetII
	{
		if (size_or_type == EthernetII::IP) 
			return data.remove( 0, sizeof( HEADER_802_3 ) );

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
			PRINT_DEBUG << "Ethernet SNAP";
			if (qFromBigEndian( snap_header->ethertype ) == EthernetII::IP)
				return data.remove( 0, sizeof( HEADER_802_3 ) + sizeof( SNAP_HEADER ) );
			break;
		}

		case SAP_IP://802.2 + 802.3
			/* Cannot be done as there is no SAP assigned to ARP protocol */
			PRINT_DEBUG << "802.2/802.3";
			return data.remove( 0, sizeof( HEADER_802_3 ) + sizeof( SAP_HEADER ) );
	}
	return QByteArray();
}
