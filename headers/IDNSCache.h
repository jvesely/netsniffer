/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

class IDNSCache: public QObject
{
	Q_OBJECT

public:
	inline const QString operator [] ( const QHostAddress& address) const
		{ return translate( address ); }
	~IDNSCache() {};

	virtual const QString translate( const QHostAddress& address ) const = 0;
	virtual bool insert( const QHostAddress& address, const QString& name ) = 0;

signals:
	void newEntry( const QHostAddress& address, const QString& name );
};
