#ifndef __UTILS_H__
#define __UTILS_H__

// for C++11 compatability. Use only with -std=c++11
#define Rwrap(a, b, c) Ge##a(b, c)
#define RGenMT19937(x, y) Rwrap(tRandom, y, x)

int GetRandom(int min, int max);

#endif