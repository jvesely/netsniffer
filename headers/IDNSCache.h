/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

/*!
 * @class IDNSCache IDNSCache.h "IDNSCache.h"
 * @brief DNSCache interface class.
 *
 * Provides interface for the DNSCache implementation.
 * Inserting entries, retrieving entries and new entry notification.
 */
class IDNSCache: public QObject
{
	Q_OBJECT

public:
	/*!
	 * @brief virtual destructor for the sake of inheritence.
	 */
	virtual ~IDNSCache() {};

	/*!
	 * @brief Translates IP address into stored DNS name.
	 * @param address IP address to translate.
	 * @return Stored DNS name belonging to the input ipaddress,
	 * if no such name is stored, QString representation of the ipaddress 
	 * is returned.
	 */
	virtual const QString translate( const QHostAddress& address ) const = 0;

	/*!
	 * @brief Overloaded function provided for convenience.
	 * @param address IP address to translate.
	 * @return DNS name assigned to the address or the string format of the
	 * address.
	 * @note See translate for details.
	 */
	inline const QString operator [] ( const QHostAddress& address) const
		{ return translate( address ); }

	/*!
	 * @brief Inserts pair of address and associated name into the cache.
	 * @param address Addresss to insert.
	 * @param name Assocaited anme to insert.
	 * @return True if pair was succesfully inserted, false otherwise.
	 * @note Signal newEntry shall be emitted if the insertion was sucessfull.
	 * @note Inserting new entry might cause some older entries 
	 * to be removed from the cache.
	 */
	virtual bool insert( const QHostAddress& address, const QString& name ) = 0;

signals:
	/*!
	 * @brief Signals change in the content of the cache.
	 * @param address newly added address.
	 * @param name newly added DNS name.
	 */
	void newEntry( const QHostAddress& address, const QString& name );
};
