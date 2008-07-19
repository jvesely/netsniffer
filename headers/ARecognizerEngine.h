#ifndef _ARECOGNIZERENGINE_h_
#define _ARECOGNIZERENGINE_h_

#include "IConnection.h"

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
	virtual bool recognize(QByteArray dataIn, QByteArray dataOut, QHostAddress addrSrc, QHostAddress addrDest, quint16 portSrc, quint16 portDest, int proto) const = 0;
	virtual QPair<QString, QString> quickLook(const IConnection * conn) const = 0;
	virtual QWidget * getOptionsPage() const = 0;
signals:
	void recognized() const;
	void dnsRecord(QHostAddress, QString) const;
};

Q_DECLARE_INTERFACE(ARecognizerEngine,
											"Recognizer Engine/1.0.0")
#endif

