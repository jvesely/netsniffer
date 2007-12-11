#include "cMACheader.h"
#include <QString>



/*-------------------------------------------------------------------------------*/
cMACheader::operator QString(){


	QString out("Src MAC: ");
	out.append(QByteArray(srcaddr,6).toHex());
	out.append("\nDest MAC: ");
	out.append(QByteArray(destaddr,6).toHex());

	
	out.append(QByteArray(type,2).toHex());
	out.append("\n");
	return out;

}