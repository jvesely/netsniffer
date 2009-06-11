#pragma once
#include "IConnection.h"
#include "struct/SafeHash.h"

template class SafeHash<const IConnection*, QStringList >;

class CommentStore:
	public QObject,
	public SafeHash<const IConnection*, QStringList >
{
	Q_OBJECT

public:
	inline QStringList& operator[]( const IConnection* connection );
	inline int remove( const IConnection* connection );

signals:
	void changed( const IConnection* );	
};
/*----------------------------------------------------------------------------*/
/* DEFINITIONS ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
inline QStringList& CommentStore::operator[]( const IConnection* connection )
{
	emit changed( connection );
	return this->SafeHash<const IConnection*, QStringList >::operator[]( connection );
}
/*----------------------------------------------------------------------------*/
inline int CommentStore::remove( const IConnection* connection )
{
	emit changed( connection );
	return this->SafeHash<const IConnection*, QStringList >::remove( connection );
}
