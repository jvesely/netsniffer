#pragma once

class IDNSCache: public QObject
{
	Q_OBJECT;

public:
	virtual const QString translate( const QHostAddress address ) const = 0;
	inline const QString operator [] ( const QHostAddress address) const
		{ return translate( address ); }
	~IDNSCache() {};

	virtual bool insert( const QHostAddress address, const QString name ) = 0;

signals:
	virtual void newEntry( const QHostAddress address, const QString name );
};
