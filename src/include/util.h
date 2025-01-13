#ifndef __UTIL_H
#define __UTIL_H
#include <x86intrin.h>

extern "C" void _init(void);
extern "C" void _fini(void);

char *itoa(int, int, char *);
void printk(const char *, ...);
void panic(const char *);
void set_display_position(int, int);

#endif