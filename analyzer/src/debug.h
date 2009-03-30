#ifndef DEBUG_TEXT
#define DEBUG_TEXT "[ Undefined debug in file "__FILE__" ]:"
#endif
#define PRINT_DEBUG qDebug() << DEBUG_TEXT
