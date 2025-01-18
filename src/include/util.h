#ifndef __UTIL_H
#define __UTIL_H

#include <x86intrin.h>

#define MHZ 85

extern "C" void _init(void);
extern "C" void _fini(void);

template <class T> char *itoa(T, int, char *);
void printk(const char *, ...);
void panic(const char *);
void set_display_position(int, int);

inline unsigned long long get_timer() {
  /*unsigned long long x = __rdtsc();
  unsigned long long y = __rdtsc();
  printk("diff x y %l %l\n", x, y);*/
  return __rdtsc();
};

void delay(int);

#endif