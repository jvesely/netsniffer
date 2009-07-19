/*!
 * @file
 * @brief IRecognizer interface class.
 *
 * IRecognizer class shall provide interface for the manipulation with
 * data recognizers. The class should identify itself by name and be able to
 * analyze connections and present result sof this analysis.
 */
#pragma once

class IConnection;

/*!
 * @class IRecognizer IRecognizer.h "IRecognizer.h"
 * @brief Recognizer interface class.
 *
 * Provides interface for Recognizer use and manipulation. Recognzier shall
 * identify itself by name and be able to handle connection analysis.
 */
class IRecognizer: public QObject
{
public:
	/*!
	 * @brief Retrieves recgonizer name.
	 */
	virtual const QString name() const = 0;

	/*!
	 * @brief Determines whether the recognizer would be able to analyze data.
	 * @param connection Pointer to the connection to be analyzed.
	 * @return True if the recognizer is recognized and will be able to 
	 * analyze data.
	 * @note The result of this function does not have to provide definite answer
	 * to the question of beeing able to analyze the data.
	 * @note This function does not change input connection in any way.
	 */
	virtual bool guess( const IConnection* connection ) = 0;

	/*!
	 * @brief Analyzes data of the connection.
	 * @param connection Connection to be analyzed.
	 * 
	 * Retrives next packet from the connection and processes it in any way 
	 * necessary.
	 */
	virtual bool parse( IConnection* connection ) = 0;

	/*!
	 * @brief Retrieves shorrt comment on the data in the connection.
	 * @param connection Connection to comment on.
	 * @return Comment produced by the recognizer.
	 */
	virtual QVariant comment( IConnection* connection ) = 0;

	/*!
	 * @brief Display user dialog containg detailed information on the connection.
	 * @param connection Conneciton to view in detail.
	 * @return True if datial dialog was successfully displayed, false otherwise.
	 */
	virtual bool showDetails( IConnection* connection ) = 0;
};
