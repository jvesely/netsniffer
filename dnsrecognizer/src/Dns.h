/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

//RFC 1035
namespace Dns
{
	enum Type {
#define TYPE(name,value) \
		name = value,
#include "dns_types.h"
#undef TYPE
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
		quint16 QTYPE:16;
		quint16 QCLASS:16;
		quint16 TTL_HIGH:16;
		quint16 TTL_LOW:16;
		quint16 DATA_LENGTH:16;
	};

	static const QString opcodeToString( quint8 opcode )
	{
			static const QString operation_codes[] = {
				"Standard query",
				"Inverse query",
				"Status query",
				"Notify",
				"Update"
			};
			return (opcode > 4) ? "Reserved OpCode" : operation_codes[opcode];
	}

	static const QString classToString( quint16 qclass )
	{
		static const QString class_codes[] = {
			"Internet",  // IN
			"CSNET",     // CS
			"Chaos",     // SH
			"Hesiod",    // HS
			"Any"        // *  --value is 0xff
		};
		switch (qclass) {
			case 1:
			case 2:
			case 3:
			case 4:
				return class_codes[ qclass -1 ];
			case 255:
				return class_codes[ 4 ];
		}
		return "Reserved";
	}

	static const QHash<Type, QString> types()
	{
		QHash<Type, QString> result;
#define TYPE( name, value )\
		result.insert( (Type)value, #name );
#include "dns_types.h"
#undef TYPE

		return result;
	}

	static const QString typeToString( Type type )
	{
		static const QHash<Type, QString> cache = types();
		return cache.value( type, "Unknown type" );
	}
};
