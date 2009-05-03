#pragma once

struct IPv4Header
{
	quint8 header_length:4;
	quint8 version:4;  // exchanged because of the aligning

	quint8 tos:8;
	quint16 total_length:16;
	quint16 identification:16;

	quint16 fragment_offset:13;
	quint16 flags:3;  // exchanged because of the aligning

	quint8 ttl:8;
	quint8 protocol:8;
	quint16 header_checksum:16;
	quint32 source_address:32;
	quint32 destination_address:32;
};

struct UDPHeader
{
	quint16 source_port:16;
	quint16 destination_port:16;
	quint16 length:16;
	quint16 checksum:16;
};

struct TCPHeader
{
	quint16 source_port:16;
	quint16 destination_port:16;
	quint32 sequence:32;
	quint32 acknowledgement:32;
	quint8 :4;
	quint8 header_length:4;
	bool fin:1;
	bool syn:1;
	bool rst:1;
	bool psh:1;
	bool ack:1;
	bool urg:1;
	quint8 :2;
	quint16 window_size:16;
	quint16 checksum:16;
	quint16 urgent_pointer:16;
};
