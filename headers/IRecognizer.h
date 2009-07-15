/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
