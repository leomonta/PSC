#pragma once


#ifndef TYPE
#define TYPE int
#endif

#define rename(name, t) name##_##t
// double macro expension yay!
#define rn(x, T) rename(x, T)

#define r(x) rn(x, TYPE)
