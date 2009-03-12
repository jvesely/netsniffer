#pragma once

#include "IConnection.h"

class ARecognizerEngine:public QObject
{
	Q_OBJECT

public:
	inline virtual ~ARecognizerEngine() {};
	virtual	QString name() const = 0;
	virtual QString info() const = 0;

public slots:
	virtual QWidget * analyze(const IConnection * conn)  = 0;
	virtual QPair<QString, QString> quickLook(const IConnection * conn) const = 0;
	virtual QWidget * getOptionsPage() const = 0;

signals:
	void recognized() const;
	void dnsRecord(QHostAddress, QString) const;
};

Q_DECLARE_INTERFACE(ARecognizerEngine,
											"Recognizer Engine/1.0.0")
