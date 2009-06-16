#pragma once

class IConnection;

class IRecognizer: public QObject
{
public:
	virtual ~IRecognizer(){};
	virtual const QString name() const = 0;
	virtual bool guess( const IConnection* connection ) = 0;
	virtual bool parse( IConnection* connection ) = 0;
	virtual QVariant comment( IConnection* connection ) = 0;
	virtual bool showDetails( IConnection* connection ) = 0;
};
