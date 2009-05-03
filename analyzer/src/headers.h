#pragma once

struct IPv4Header
{
	uint version:4;
	uint header_length:4;
	uint tos:8;
	uint total_length:16;
	uint identification:16;
	uint flags:3;
	uint fragment_offset:13;
	uint ttl:8;
	uint protocol:8;
	uint header_checksum:16;
	uint source_address:32;
	uint destination_address:32;
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
	uint source_port:16;
	uint destination_port:16;
	uint sequence:32;
	uint acknowledgement:32;
	uint header_length:4;
	uint :6;
	uint urg:1;
	uint ack:1;
	uint psh:1;
	uint rst:1;
	uint syn:1;
	uint fin:1;
	uint window_size:16;
	uint checksum:16;
	uint urgent_pointer:16;
};
