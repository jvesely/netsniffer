#pragma once

template<class T>
class SafeQueue: private QQueue<T>{
	QWaitCondition noData;
	QMutex dataGuard;
public:
	void enqueue(T item){
		QMutexLocker lock(&dataGuard);
		
		QQueue<T>::enqueue(item);
		noData.wakeOne();
	}
	T dequeue(){
		QMutexLocker lock(&dataGuard);
		if (QQueue<T>::empty())
			noData.wait(&dataGuard);
		

		if (QQueue<T>::empty()){ // aditional check should happen only on close
			throw std::runtime_error("EMPTY QUEUE");
		}
		return QQueue<T>::dequeue();
	}
	int count(){
		QMutexLocker lock(&dataGuard);
		return QQueue<T>::count();
	}
	bool empty(){
		return count() == 0;
	}
	void release(){
		noData.wakeAll();
	}
};
