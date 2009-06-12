#pragma once
#include "struct/SafeHash.h"

class Connection;

template class SafeHash<const Connection*, QVariant >;

class CommentStore:
	public QObject,
	public SafeHash<const Connection*, QVariant >
{
	Q_OBJECT

public:
	inline QVariant& operator[]( const Connection* connection );
	inline int remove( const Connection* connection );

signals:
	void changed( const Connection* );	
};
/*----------------------------------------------------------------------------*/
/* DEFINITIONS ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
inline QVariant& CommentStore::operator[]( const Connection* connection )
{
	QVariant& ret = this->SafeHash<const Connection*, QVariant >::operator[]( connection );
	emit changed( connection );
	return ret;
}
/*----------------------------------------------------------------------------*/
inline int CommentStore::remove( const Connection* connection )
{
	emit changed( connection );
	return this->SafeHash<const Connection*, QVariant >::remove( connection );
}
