/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

class IAnalyzer;

class IPlugin
{
public:
	virtual bool init( IAnalyzer* app ) = 0;
	virtual ~IPlugin() {};
};

Q_DECLARE_INTERFACE(IPlugin, "Generic Plugin Interface/1.0");
