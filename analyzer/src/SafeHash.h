#pragma once

template<typename K, typename T>
class SafeHash: public QHash<K, T>{
	QMutex guard;
public:
	T& operator[](K item){
		QMutexLocker lock(&guard);
		return QHash<K,T>::operator[](item);
	}
};
