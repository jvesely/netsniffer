#pragma once

//RFC 1035
namespace Dns
{
static const QString OperationCodes[] = {
	"Standard query",
	"Inverse query",
	"Status query",
	"Notify",
	"Update",
	"Reserved"   // --all other
};

static const QString QClass[] = {
	"Internet",  // IN
	"CSNET",     // CS
	"Chaos",     // SH
	"Hesiod",    // HS
	"Any"        // *  --value is 0xff
};

enum QType = {
	A = 1,        // host address
	NS,       // authoritative nameserver
	MD,       // mail destination
	MF,       // mail forwarder
	CNAME,    // canonical name
	SOA,      // Start Of Authority
	MB,       // mail box
	MG,       // mail group
	MR,       // mail rename
	NUL,     // NULL RR (???)
	WKS,      // Well Known Services
	PTR,      // name pointer
	HINFO,    // hostinformantion
	MX,       // mail exchange
	TXT,      // text
	RP,       // responsible person
	AFSDB,    // database servers for AFS
	X25,      // X.25
	ISDN,     // ISDN Number
	RT,       // ???
	NSAP,     // ???
	NSAP-PTR, // ???
	SIG,      // Signature -- RFC 2535
	KEY,      // Key record -- RFC 4034
	PX,       // ???
	GPOS,     // ???
	AAAA,     // IPv6 address -- RFC 3596
	LOC,      // Location -- RFC 1876
	NXT,      // ??? -- RFC 3755
	//30 ^
	EID,      // ???
	NIMLOC,   // ???
	SRV,      // Service location  -- RFC 2782
	ATMA,     // ???
	NAPTR,    // Naming Authority ptr -- RFC 3403
	KX,       // Old DNSSEC -- RFC 2230/2065
	CERT,     // Certificate -- RFC 4398
	A6,       // Old IPv6 -- RFC 3363
	DNAME,    // Delegation Name -- RFC 2672
	SINK,     // Kitchen Sink
	OPT,      // Option -- RFC 2671
	APL,      // ???
	DS,       // Delegation Signer -- RFC 4034
	SSHFP,    // SSH FingerPrint -- RFC 4255
	IPSECKEY, // IPSEC key -- RFC 4025


	AFXR,  // request entire zone
	MAILB, // mailbox-related
	MAILA, // mail-agent RR
	*      // all records
};

struct Header
{
	quint16 ID:16; // 16 bit identifier
	
	quint8 RD:1;
	quint8 TC:1;
	quint8 AA:1;
	quint8 Opcode:4; // opcode
	quint8 QR:1;  // dnsquery(1), dnsanswer(0)

	quint8 RCODE:4;
	quint8 Z:3;
	quint8 RA:1;

	quint16 QDCOUNT:16; // number of question entries
	quint16 ANCOUNT:16; // number of answer entries
	quint16 NSCOUNT:16; // number of NS resource records
	quint16 ARCOUNT:16; // number of additional records
};

struct QuestionData
{
	quint16 QTYPE:16;
	quint16 QCLASS:16;
};

struct AnswerData
{
};

}
