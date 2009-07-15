/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
