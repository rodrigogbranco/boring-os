#ifndef __UTIL_H
#define __UTIL_H

namespace Util {

char *itoa(int, int, char *);
void printk(const char *, ...);
void panic(const char *);

} // namespace Util

#endif