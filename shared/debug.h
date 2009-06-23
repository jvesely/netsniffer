#ifndef QT_NO_DEBUG

#ifndef DEBUG_TEXT
#define DEBUG_TEXT "[ Undefined debug in file "__FILE__" ]:"
#endif

#define PRINT_DEBUG(arg)  qDebug() << DEBUG_TEXT << arg

#else
#define PRINT_DEBUG(arg)
#endif
