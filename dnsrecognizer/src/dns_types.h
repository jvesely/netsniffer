TYPE(A, 1)        // host address
TYPE(NS, 2)           // authoritative nameserver
TYPE(MD, 3)           // mail destination
TYPE(MF, 4)           // mail forwarder
TYPE(CNAME, 5)        // canonical name
TYPE(SOA, 6)          // Start Of Authority
TYPE(MB, 7)           // mail box
TYPE(MG, 8)           // mail group
TYPE(MR, 9)           // mail rename
TYPE(NUL, 10)          // NULL RR (???)
TYPE(WKS, 11)          // Well Known Services
TYPE(PTR, 12)          // name pointer
TYPE(HINFO, 13)        // hostinformantion
TYPE(MX, 14)           // mail exchange
TYPE(TXT, 15)          // text
TYPE(RP, 16)           // responsible person
TYPE(AFSDB, 17)        // database servers for AFS
TYPE(X25, 18)          // X.25
TYPE(ISDN, 19)         // ISDN Number
TYPE(RT, 20)           // ???
TYPE(NSAP, 21)         // ???
TYPE(NSAPPTR, 22)      // ???
TYPE(SIG, 23)          // Signature -- RFC 2535
TYPE(KEY, 24)          // Key record -- RFC 4034
TYPE(PX, 25)           // ???
TYPE(GPOS, 26)         // ???
TYPE(AAAA, 27)         // IPv6 address -- RFC 3596
TYPE(LOC, 28)          // Location -- RFC 1876
TYPE(NXT, 29)          // ??? -- RFC 3755
TYPE(EID, 30)          // ???
TYPE(NIMLOC, 31)       // ???
TYPE(SRV, 32)          // Service location  -- RFC 2782
TYPE(ATMA, 33)         // ???
TYPE(NAPTR, 34)        // Naming Authority ptr -- RFC 3403
TYPE(KX, 35)           // Old DNSSEC -- RFC 2230/2065
TYPE(CERT, 36)         // Certificate -- RFC 4398
TYPE(A6, 37)           // Old IPv6 -- RFC 3363
TYPE(DNAME, 38)        // Delegation Name -- RFC 2672
TYPE(SINK, 39)         // Kitchen Sink
TYPE(OPT, 40)          // Option -- RFC 2671
TYPE(APL, 41)          // ???
TYPE(DS, 42)           // Delegation Signer -- RFC 4034
TYPE(SSHFP, 43)        // SSH FingerPrint -- RFC 4255
TYPE(IPSECKEY, 44)     // IPSEC key -- RFC 4025
TYPE(RRSIG, 45)        // DNSSEC Signature -- RFC 4034
TYPE(NSEC, 46)         // next secure -- RFC 4034
TYPE(DNSKEY, 47)       // DNS Key -- RFC 4034
TYPE(DHCID, 48)        // DHCP Identifier -- RFC 4701
TYPE(NSEC3, 49)        // next secure v.3 -- RFC 5155
TYPE(NSEC3PARAM, 50)   // NSEC3 parameters -- RFC 5155
TYPE(HIP, 55)          // Host Identification Protocol -- RFC 5205
TYPE(SPF, 99)          // SPF record -- RFC 4404

TYPE(		TKEY, 249)     // Transaction Key -- RFC 2930
TYPE(		TSIG, 250)     // Transaction Signature -- RFC 2845
TYPE(		IXFR, 251)     // Incremental zone transfer -- RFC 1995
TYPE(		AFXR, 252)     // request entire zone
TYPE(		MAILB, 253)    // mailbox-related
TYPE(		MAILA, 254)    // mail-agent RR
TYPE(		ASTERISK, 255) // all records
TYPE(		TA, 32768)      // DNSSEC Trust Authorities,
