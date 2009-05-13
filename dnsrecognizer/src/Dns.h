#pragma once

//RFC 1035
namespace Dns
{

	static const QString Class[] = {
		"Internet",  // IN
		"CSNET",     // CS
		"Chaos",     // SH
		"Hesiod",    // HS
		"Any"        // *  --value is 0xff
	};

	enum Type {
#define TYPE(name,value) \
		name = value,
#include "dns_types.h"
#undef TYPE
	};

	Q_ENUMS (Type);

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

	static const QString opcodeToString( quint8 opcode )
	{
			static const QString OperationCodes[] = {
				"Standard query",
				"Inverse query",
				"Status query",
				"Notify",
				"Update"
			};
			if (opcode > 4) {
				return "Reserved OpCode";
			} else {
				return OperationCodes[opcode];
			}
	}

	static const QString typeToString( Type type )
	{
		static QHash<Type, QString> convertor;
		if (convertor.isEmpty())
		{
#define TYPE( name, value )\
			convertor.insert( (Type)value, #name );
#include "dns_types.h"
#undef TYPE
		}

		return convertor.value( type, "Unknown type" );
	}
};
