#pragma once


class IRecognizer: public QObject
{
	Q_OBJECT	
public:
	IRecognizer(){};
	virtual ~IRecognizer(){};
//	virtual const QString fileName() const = 0;
	virtual const QString name() const = 0;
};
