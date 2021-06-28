#ifndef TTT_UTIL_DEBUG_H
#define TTT_UTIL_DEBUG_H

#include <iostream>

#ifdef TTT_DEBUG

#define TTT_COUT(x) std::cout << x << '\n'
#define TTT_CERR(x) std::cerr << x << '\n'
#define TTT_CLOG(x) std::clog << x << '\n'

#else

#define TTT_COUT(x)
#define TTT_CERR(x)
#define TTT_CLOG(x)

#endif

#endif