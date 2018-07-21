#ifndef ODGL_LOGGING_H
#define ODGL_LOGGING_H

#include <iostream>

#ifdef ODGL_LOG_SCOPE
#define ODGL_SCOPE() std::cout << "Entering " << __func__ << std::endl
#else
#define ODGL_SCOPE()
#endif

#ifdef ODGL_LOG_INFO
#define ODGL_INFO(x) std::cout << "ODGL INFO:" << x << std::endl
#else
#define ODGL_INFO(x)
#endif

#endif