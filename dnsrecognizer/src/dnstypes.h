#pragma once

enum DNSType
{
	type_ZERO, type_A, type_NS, type_MD, type_MF,	type_CNAME, type_SOA,
	type_MB, type_MG, type_MR, type_NULL, type_WKS, type_PTR, type_HINFO,
	type_MINFO, type_MX, type_TXT, type_RP, type_AFSDB, type_X25, type_ISDN,
	type_RT, type_NSAP, type_NSAP_PTR, type_SIG, type_KEY, type_PX, type_GPOS,
	type_AAAA, type_LOC, type_NXT, type_EID, type_NB, type_SRV, type_ATMA,
	type_NAPTR, type_KX, type_A6, type_DNAME, type_SINK, type_OPT, type_APL,
	type_DS, type_SSHFP, type_IPSECKEY, type_RRSIG, type_NSEC, type_DNSKEY,
	type_DHCID, type_NSEC3, type_NSEC3PARAM
	//first 51 types
};
