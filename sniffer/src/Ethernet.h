#pragma once

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
	static const int SAP_IP = 0x06;
}
