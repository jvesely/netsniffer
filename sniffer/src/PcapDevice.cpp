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
	static const int MAXIMUM_SIZE = 1500;
	static const int RAW = 0xFF;
	static const int SNAP = 0xAA;
	static const int HEADER_SIZE = 14; // 2 * 6b (MAC) + 2b (type)
	static const int SNAP_HEADER_SIZE = 8;
	static const int SAP_HEADER_SIZE = 3;
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
			PRINT_DEBUG << "UNKNOWN datalink type: " << mType;
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
//	QByteArray data( data, len );
	
	// 13th and 14th byte indicates either size or EthernetII types
	const quint16 type_or_size = qFromBigEndian( *(quint16*)data.mid( 12, 2 ).data() );
//	const quint16 type_or_size = qFromBigEndian(* (quint16 *)(data + 12));

	using namespace Ethernet;

	if (type_or_size > MAXIMUM_SIZE) //EthernetII
	{
		const int ethertype = type_or_size;
		if (ethertype == EthernetII::IP) 
		{
			return data.remove( 0, HEADER_SIZE );
		} else {
			return QByteArray();
		}
	}

	const quint8 dsap = data.at( HEADER_SIZE );
	switch (dsap)
	{
		case RAW: 
			break; // can only carry IPX packets
		case SNAP:
		{
			/*
			 * Linux doesn't handle IP in SNAP and the bridge netfilter code also doesn't.
			 */
			Q_ASSERT ((quint8)data.at( HEADER_SIZE + 1 ) == SNAP); //ssap should be AA too
			PRINT_DEBUG << "Ethernet SNAP";
			// 21st and 22nd byte contain EthernetII ethertype value
			const int ethertype = qFromBigEndian( *(quint16*)data.mid( 20, 2 ).data() );
			if (ethertype == EthernetII::IP)
			{
				return data.remove( 0, HEADER_SIZE + SNAP_HEADER_SIZE );
			}
			else
				return QByteArray();
		}

		default ://802.2 + 802.3
			/* Cannot be done as there is no SAP assigned to ARP protocol */
			if (data.at( 14 ) == SAP_IP)
			{
				PRINT_DEBUG << "802.2/802.3";
				return data.remove( 0, 17 );
			}
	}
	return QByteArray();
}
