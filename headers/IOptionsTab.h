#pragma once

class QWidget;
class QIcon;

class IOptionsTab
{
public:
  virtual const QString name() const = 0;
  virtual const QIcon* icon() const
    { return NULL; };
  virtual bool deploy( QWidget* )  = 0;
};
