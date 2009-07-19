/*!
 * @file
 * @brief IPlugin class interface.
 *
 * Provides the basic interrface for plugins. This interface includes only
 * initializing function. This function should initialize plugin to 
 * usable state.
 */
#pragma once

class IAnalyzer;
/*!
 * @class IPlugin IPlugin.h "IPlugin.h"
 * @brief General plugin interface class.
 *
 * Provides basic plugin interface.
 */
class IPlugin
{
public:
	/*!
	 * @brief Shall initialize plugin to usable state.
	 * @param app Pointerr to the global Analzyer instance.
	 * @return True if the plugin was successfully initialized, false otherwise.
	 */
	virtual bool init( IAnalyzer* app ) = 0;

	/*!
	 * @brief Virtual destructor for the sake of inheritance.
	 */
	virtual ~IPlugin() {};
};

Q_DECLARE_INTERFACE(IPlugin, "Generic Plugin Interface/1.0");
