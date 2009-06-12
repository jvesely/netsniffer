#pragma once

class IConnection;

class IRecognizer
{
public:
	
	virtual ~IRecognizer(){};
	virtual const QString name() const = 0;
	virtual bool parse( QVariant* comments, IConnection* connection ) = 0;
};
