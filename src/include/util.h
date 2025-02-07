#ifndef __UTIL_H
#define __UTIL_H

#include <x86gprintrin.h>

#define MHZ 85

template <class T> char *itoa(T, int, char *);
void printk(const char *, ...);
void panic(const char *);
void set_display_position(int, int);

inline unsigned long long get_timer() { return __rdtsc(); };

void delay(int);

#endif