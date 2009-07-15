#pragma once

template<typename K, typename T>
class SafeHash: private QHash<K, T>
{
public:
	SafeHash() {};
	T& operator[]( const K& key );
	const T operator[]( const K& key ) const;
	T value( const K& key, const T& default_value = T() ) const;
	int remove( const K& key );
	int count() const;
	bool contains( const K& key ) const;
	QList<T> values() const;
	QList<K> keys() const;

private:
	Q_DISABLE_COPY (SafeHash);
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
const T SafeHash<K, T>::operator[]( const K& key ) const
{
	QReadLocker lock( &m_guard );
	return QHash<K,T>::operator[]( key );
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
T SafeHash<K, T>::value( const K& key, const T& default_value ) const
{
	QReadLocker lock( &m_guard );
	return QHash<K,T>::value( key, default_value );
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
int SafeHash<K, T>::remove( const K& key)
{
	QWriteLocker lock( &m_guard );
	return QHash<K, T>::remove( key );
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
int SafeHash<K, T>::count() const
{
	QReadLocker lock( &m_guard );
	return QHash<K, T>::count();
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
bool SafeHash<K, T>::contains( const K& key ) const
{
	QReadLocker lock( &m_guard );
	return QHash<K, T>::contains( key );
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
QList<T> SafeHash<K, T>::values() const
{
	QReadLocker lock( &m_guard );
	return QHash<K, T>::values();
}
/*----------------------------------------------------------------------------*/
template<typename K, typename T>
QList<K> SafeHash<K, T>::keys() const
{
	QReadLocker lock( &m_guard );
	return QHash<K, T>::keys();
}
