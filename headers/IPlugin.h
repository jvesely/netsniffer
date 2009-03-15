#pragma once

class QWidget;

class IPlugin
{
public:
	virtual bool init() = 0;
	virtual ~IPlugin() {};
};

Q_DECLARE_INTERFACE(IPlugin, "Generic Plugin Interface/1.0");
