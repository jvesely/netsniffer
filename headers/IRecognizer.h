#pragma once

class IConnection;

class IRecognizer
{
public:
	
	typedef QPair<QString, QString> QuickResult;

	virtual ~IRecognizer(){};
	virtual const QString name() const = 0;
	virtual const QuickResult quickLook( const IConnection* connection ) const = 0;
};
