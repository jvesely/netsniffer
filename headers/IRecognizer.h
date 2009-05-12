#pragma once


class IRecognizer: public QObject
{
	Q_OBJECT	
public:
	IRecognizer(){};
	virtual ~IRecognizer(){};
	virtual const QString name() const = 0;
};
