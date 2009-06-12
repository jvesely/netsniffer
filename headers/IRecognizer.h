#pragma once

class IConnection;

class IRecognizer
{
public:
	
	virtual ~IRecognizer(){};
	virtual const QString name() const = 0;
	virtual bool parse( QStringList* comments, IConnection* connection ) = 0;
};
