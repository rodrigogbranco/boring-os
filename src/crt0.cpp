#include "include/syslib.h"
#include "include/util.h"

extern int main();

extern "C" void _start() {
  _init();
  main();
  _fini();
  exit();
}