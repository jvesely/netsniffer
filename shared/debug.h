/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#ifndef QT_NO_DEBUG

#ifndef DEBUG_TEXT
#define DEBUG_TEXT "[ Undefined debug in file "__FILE__" ]:"
#endif

#define PRINT_DEBUG(arg)  qDebug() << DEBUG_TEXT << arg

#else
#define PRINT_DEBUG(arg)
#endif
