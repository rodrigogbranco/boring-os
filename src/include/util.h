#ifndef __UTIL_H
#define __UTIL_H

#include <x86intrin.h>

extern "C" void _init(void);
extern "C" void _fini(void);

template <class T> char *itoa(T, int, char *);
void printk(const char *, ...);
void panic(const char *);
void set_display_position(int, int);

inline unsigned long long get_timer() { return __rdtsc(); };

#endif