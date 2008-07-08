#ifndef _ARECOGNIZERENGINE_h_
#define _ARECOGNIZERENGINE_h_

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QHostAddress>
#include <QCache>

class ARecognizerEngine:public QObject{
	Q_OBJECT
public:
	inline virtual ~ARecognizerEngine() {};
	virtual QString getInfo() const = 0;
public slots:
	virtual bool recognize(QByteArray dataIn, QByteArray dataOut, QHostAddress addrSrc, QHostAddress addrDest, quint16 portSrc, quint16 portDest, int proto) = 0;
	virtual bool quickLook(QByteArray dataIn, QByteArray dataOut, QHostAddress addrSrc, QHostAddress addrDest, quint16 portSrc, quint16 portDest, int proto) = 0;
	virtual bool setDNS(QCache<QHostAddress, QString> * dns) = 0;
	virtual QWidget * getOptionPage() = 0;
signals:
	virtual void recognized();
};
#endif

