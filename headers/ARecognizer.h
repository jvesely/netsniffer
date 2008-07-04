#ifndef _ARECOGNIZER_h_
#define _ARECOGNIZER_h_

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QHostAddress>

class ARecognizer:public QObject{
	Q_OBJECT
public:
	inline virtual ~ARecognizer() {};
	virtual QString getInfo() const = 0;
public slots:
	virtual bool recognize(QByteArray dataIn, QByteArray dataOut, QHostAddress addrSrc, QHostAddress addrDest, quint16 portSrc, quint16 portDest, int proto)=0;
	virtual bool quickLook(QByteArray dataIn, QByteArray dataOut, QHostAddress addrSrc, QHostAddress addrDest, quint16 portSrc, quint16 portDest, int proto)=0;
signals:
	virtual void recognized();
};
#endif

