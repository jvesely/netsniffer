#pragma once

template<typename K, typename T>
class SafeHash: private QHash<K, T>
{
public:
	T& operator[]( const K& key );
	int count() const;

private:
	mutable QReadWriteLock m_guard;
};

/*----------------------------------------------------------------------------*/
/* DEFINITIONS ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
T& SafeHash<K, T>::operator[]( const K& key )
{
	QWriteLocker lock( &m_guard );
	return QHash<K,T>::operator[]( key );
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
int SafeHash<K, T>::count() const
{
	QReadLocker lock( &m_guard );
	return QHash<K, T>::count();
}
