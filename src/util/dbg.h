#ifndef DBG_H
#define DBG_H

#include <stdio.h>

#ifdef DEBUG
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define DBG(formatStr, args...) printf( ("DBG:" __FILE__ ":" TOSTRING(__LINE__) ": " formatStr "\n"), ##args)

#else
#define DBG(formatStr, args...) 
#endif


#endif

