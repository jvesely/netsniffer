#pragma once

template<class T>
class SafeQueue: private QQueue<T>{
	QSemaphore sem;
	QMutex dataGuard;
public:
	void enqueue(T item){
		QMutexLocker lock(&dataGuard);
		QQueue<T>::enqueue(item);
	//	sem.release();
	}
	T dequeue(){
//		sem.acquire();
		QMutexLocker lock(&dataGuard);
		if (QQueue<T>::empty())
			throw std::runtime_error("EMPTY QUEUE");
		return QQueue<T>::dequeue();
	}
	int count(){
		QMutexLocker lock(&dataGuard);
		return QQueue<T>::count();
	}
	bool empty(){
		return count() == 0;
	}
	void release(int n = 1){
		sem.release(n);
	}
};
