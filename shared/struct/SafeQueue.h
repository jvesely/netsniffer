/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once
#include <stdexcept>

template<class T>
class SafeQueue: private QQueue<T>
{
public:
	void enqueue( const T& item )
	{
		QWriteLocker lock( &mGuard );
		QQueue<T>::enqueue( item );
	}
/*----------------------------------------------------------------------------*/
	T dequeue() throw (std::underflow_error)
	{
		QWriteLocker lock( &mGuard);

		if (QQueue<T>::empty())
			throw std::underflow_error( "EMPTY QUEUE" );
		
		return QQueue<T>::dequeue();
	}
/*----------------------------------------------------------------------------*/
	int count() const
	{
		QReadLocker lock( &mGuard );
		return QQueue<T>::count();
	}
/*----------------------------------------------------------------------------*/
	bool isEmpty() const
	{
		QReadLocker lock( &mGuard );
		return QQueue<T>::isEmpty();
	}
/*----------------------------------------------------------------------------*/
	T& head() throw (std::underflow_error)
	{
		QWriteLocker lock( &mGuard );
		
		if (QQueue<T>::empty())
			throw std::underflow_error( "EMPTY QUEUE" );
		
		return QQueue<T>::head();
	}
/*----------------------------------------------------------------------------*/
	const T& head() const throw (std::underflow_error)
	{
		QReadLocker lock( &mGuard );
		
		if (QQueue<T>::empty())
			throw std::underflow_error( "EMPTY QUEUE" );
		
		return QQueue<T>::head();
	}

private:
	mutable QReadWriteLock mGuard;
};
