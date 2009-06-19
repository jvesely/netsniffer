#include "PcapDevice.h"

#define DEBUG_TEXT "[ Pcap Device ]: "
#define PRINT_DEBUG qDebug() << DEBUG_TEXT
/*----------------------------------------------------------------------------*/
static const int EtherII_IP = 0x0800;
static const int EtherII_IPv6 = 0x86DD;
static const int Ether_RAW = 0xFFFF;
static const int Ether_SNAP = 0xAAAA;
static const int Ether_SAP_IP = 6;
static const int Ether_SAP_IPv6 = 0; //
static const int READ_TIMEOUT = 100; //ms
static const int SNAP_LENGTH = 0x10000;
/*----------------------------------------------------------------------------*/
QString PcapDevice::translateName( const QString& name )
{
#ifdef Q_OS_WIN32
	static const QString prefix = "\\Device\\NPF_";
	static const QString registryKey = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Network\\"
"{4D36E972-E325-11CE-BFC1-08002BE10318}";

	if (name.startsWith( prefix )) {
		const QSettings registry( QString( "%1\\%2\\Connection" ).arg(
			registryKey, name.mid( prefix.length() ) ), QSettings::NativeFormat );
		return registry.value( "Name", name ).toString();
	}
#endif

	return name;
}
/*----------------------------------------------------------------------------*/
PcapDevice::PcapDevice( pcap_if_t *dev )
: handle( 0 ), pcapName( dev->name ), name( translateName( dev->name ) ), 
	desc( dev->description ), type( 0 ), capturing( false )
{}
/*----------------------------------------------------------------------------*/
PcapDevice::~PcapDevice()
{
	PRINT_DEBUG << "Device Dying";
	if (capturing)
		captureStop();
	else
		close();
}
/*----------------------------------------------------------------------------*/
pcap_t* PcapDevice::open()
{
	PRINT_DEBUG << "Opening Device...";
	char* err = 0;
	if (!handle) {
		int promisc = (pcapName == "any") ? 0 : 1; // ok serious bug here device any not working promisc, leads to crash in glibc
		handle = pcap_open_live( pcapName.toAscii().data(), SNAP_LENGTH, promisc, READ_TIMEOUT, err );
	}
	if (!handle)
		PRINT_DEBUG << "ERROR:" << err;
	else
		type = pcap_datalink( handle );
	return handle;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::close()
{
	if (handle)
		pcap_close( handle );
	handle = 0;
	type = 0;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::run()
{
	pcap_pkthdr header;
	const u_char* data;
	capturing = true;
	emit captureStarted( this );
	while (capturing) {
		if ((data = pcap_next( handle, &header )))
			packet( header, data );
	}
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStart()
{
	if (capturing || !open())
		return false;
	PRINT_DEBUG << "Starting...";
	start();
	return true;
}
/*----------------------------------------------------------------------------*/
bool PcapDevice::captureStop()
{
	capturing = false;
	terminate();
	wait();
	close();
	emit captureStopped( this );
	return !capturing;
}
/*----------------------------------------------------------------------------*/
void PcapDevice::packet( pcap_pkthdr header, const u_char* data )
{
	QByteArray load = link2IP( data, header.len );
	if (!load.isNull())
		emit packetArrived( this, load );
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::link2IP( const u_char* data, int len )
{
	switch(type){
		case DLT_NULL: //BSD loopback
			return QByteArray( (char *)data + 4, len - 4);
		case DLT_EN10MB: //Ethernet
			return ether2IP(data, len);
		default: //Other types
			return QByteArray();
	}
}
/*----------------------------------------------------------------------------*/
QByteArray PcapDevice::ether2IP( const u_char* data, int len )
{
	quint16 ethertype = qFromBigEndian(* (quint16 *)(data + 12));
	if (ethertype > 1500) {//EthernetII
		//qDebug() << "Ethernet II ethertype: " << ethertype << endl; 
		if ((ethertype == EtherII_IP) || (ethertype == EtherII_IPv6))
//return IP part of EtherrnetII frame (2xMAC(6) + ethertype(2) + //FCS(end:4)
//			qDebug() << "EthernetII";
			return QByteArray((char*) (data + 14), len - 14);
	} else { 
		quint16 diff = qFromBigEndian(*(quint16 *)(data + 14)); //next 2 bytes
//		qDebug()<< "Diff" << diff <<endl;
		
		switch (diff) {
			case Ether_RAW : // can only carries IPX packets
			//http://www.dataip.co.uk/Network/FrameTypes.php
//				qDebug() << "RAW";
				return QByteArray();
				break;
			case Ether_SNAP :
			//skip 2xMAC(6), length(2), 2xSAP(1), control(1), padding(3)
			//then follows EthernetII ethertype, at the end // FCS(4)
				ethertype = qFromBigEndian(*(quint16*)(data + 20) );
				if ((ethertype == EtherII_IP) || (ethertype == EtherII_IPv6)){
//					qDebug() << "Ethernet_SNAP";
					return QByteArray((char*)(data + 22), len - 22);
				}
			default ://802.2 + 802.3
				if (*(data + 14) == Ether_SAP_IP){
				//2xMAC(6), length(2), DSAP(1), SSAP(1), Control(1), //FSC at the end(4)
					qDebug() << "802.2/802.3";
					return QByteArray((char*)(data + 17), len - 17);
				}
		}
	}
	return QByteArray();
}
