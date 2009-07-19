/*!
 * @file
 * @brief IOptionsTab interface class.
 *
 * IOptionsTab class provides interface for options pages. It shall provide
 * it name and icon and be able to deploy to provided QWidget.
 */
#pragma once

class QWidget;
class QIcon;

/*!
 * @class IOptionsTab IOptionsTab.h "IOptionsTab.h"
 * @brief OptionsTab interface class.
 *
 * Provides interface for Options page object. Shall provide access
 * to registered name and icon, and be able to deploy the options page 
 * into the provided QWidget area upon request.
 */
class IOptionsTab
{
public:
	/*!
	 * @brief Retrieves name of the optinos page.
	 */
  virtual const QString name() const = 0;

	/*!
	 * @brief Retrieves pointer to the page icon. Default to NULL.
	 * @return Pointer to the icon that shall be used when referncing 
	 * this options page.
	 */
  virtual const QIcon* icon() const
    { return NULL; };

	/*!
	 * @brief Paints the page itself into the provided QWidget. 
	 * @param area QWidget to paint GUI on.
	 */
  virtual bool deploy( QWidget* area ) = 0;
};
