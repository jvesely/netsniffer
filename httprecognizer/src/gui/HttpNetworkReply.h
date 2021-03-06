/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "Http.h"

class HttpNetworkReply: public QNetworkReply
{
public:
	HttpNetworkReply( const QUrl& url, const Http::Response* response )
	{
		if (response)
		{
			mResponse.open( QIODevice::WriteOnly );
			QList<QPair<QString,QString> > headers = response->first.values();
			while (!headers.isEmpty())
			{
				qDebug() << "adding header" << headers.first();
				setRawHeader( headers.first().first.toAscii(), headers.first().second.toAscii() );
				headers.removeFirst();
			}
			//mResponse.write( response->first.toString().toAscii() );
			mResponse.write( *response->second );
			mResponse.close();
		}
//		qDebug() << "buffer status" << mResponse.data();
		setUrl( url );
		open( ReadOnly | Unbuffered );
		mResponse.open( ReadOnly );
		QTimer::singleShot( 0, this, SIGNAL(readyRead()) );
		QTimer::singleShot( 0, this, SIGNAL(finished()) );

	};

	virtual void abort ()	{}
	
	virtual qint64 bytesAvailable() const
		{ qDebug() << "ASKED COUNT" << mResponse.bytesAvailable(); return mResponse.bytesAvailable(); }
	
	virtual bool isSequential () const
		{ return true; }
	
	virtual qint64 readData ( char* data, qint64 maxSize )
		{qDebug() << "reqested data============================"; 
		return mResponse.read( data, maxSize ); }

private:
	QBuffer mResponse;
};
