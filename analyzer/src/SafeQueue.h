#pragma once

template<typename T>
class SafeQueue{
	QQueue<T> data;
	QSemaphore sem;
	QMutex dataGuard;
public:
	void enqueue(T item){
		QMutexLocker lock(&dataGuard);
		data.enqueue(item);
		sem.release();
	}
	T dequeue(){
		sem.acquire();
		QMutexLocker lock(&dataGuard);
		return data.dequeue();
	}
	int count(){
		QMutexLocker lock(&dataGuard);
		return data.count();
	}
	bool empty(){
		return count() == 0;
	}
	void release(int n = 1){
		sem.release(n);
	}
};
