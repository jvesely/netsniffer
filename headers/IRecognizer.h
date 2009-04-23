#pragma once


class IRecognizer: public QObject
{
	Q_OBJECT	
public:
	IRecognizer(){};
	virtual ~IRecognizer(){};
//	virtual const QString fileName() const = 0;
	virtual const QString name() const = 0;
	virtual bool isLoaded() const = 0;

signals:
	void statusChanged(IRecognizer * me);

};
