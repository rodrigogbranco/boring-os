#include "include/util.h"

void clear_screen() {
  char *currentScreenPointer = (char *)SCREEN_POINTER;
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLUMNS; j++) {
      *currentScreenPointer++ = ' ';
      *currentScreenPointer++ = CHAR_ATTR;
    }
  }
}