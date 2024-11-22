#include "include/util.h"

extern void clear_screen();

extern "C" void _start() {
  const char *kernelMsg = "Hello Protected Mode!";

  clear_screen();

  char *currentMsg = (char *)kernelMsg;
  char *currentScreenPointer = (char *)SCREEN_POINTER;
  do {
    if (*currentMsg == NULL) {
      break;
    }

    *currentScreenPointer++ = *currentMsg++;
    *currentScreenPointer++ = CHAR_ATTR;
  } while (*currentMsg != NULL);

  while (true) {
    /* BUSY LOOP*/
  }
}