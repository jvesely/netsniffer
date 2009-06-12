#pragma once
#include "struct/SafeHash.h"

class Connection;

template class SafeHash<const Connection*, QStringList >;

class CommentStore:
	public QObject,
	public SafeHash<const Connection*, QStringList >
{
	Q_OBJECT

public:
	inline QStringList& operator[]( const Connection* connection );
	inline int remove( const Connection* connection );

signals:
	void changed( const Connection* );	
};
/*----------------------------------------------------------------------------*/
/* DEFINITIONS ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
inline QStringList& CommentStore::operator[]( const Connection* connection )
{
	QStringList& ret = this->SafeHash<const Connection*, QStringList >::operator[]( connection );
	emit changed( connection );
	return ret;
}
/*----------------------------------------------------------------------------*/
inline int CommentStore::remove( const Connection* connection )
{
	emit changed( connection );
	return this->SafeHash<const Connection*, QStringList >::remove( connection );
}
