#ifndef TTT_UTIL_DEBUG_H
#define TTT_UTIL_DEBUG_H

#include <iostream>

#ifdef TTT_DEBUG

#define TTT_COUT(msg) std::cout << msg << '\n'
#define TTT_CERR(msg) std::cerr << msg << '\n'
#define TTT_CLOG(msg) std::clog << msg << '\n'

#else

#define TTT_COUT(msg)
#define TTT_CERR(msg)
#define TTT_CLOG(msg)

#endif

#endif