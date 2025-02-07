#include "include/syslib.h"

extern int main();
extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void _start() {
  _init();
  main();
  _fini();
  exit();
}