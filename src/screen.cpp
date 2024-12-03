#include "include/screen.h"
#include "include/gdt.h"

GDT test(0, 0, 0, 0);

char *current_position = (char *)SCREEN_POINTER;

inline char get_char_attr(CharColors foreground, CharColors background) {
  return (foreground & 0x0f) | (background << 4);
}

inline void reset_current_position() {
  current_position = (char *)SCREEN_POINTER;
}

void printk(char *msg, CharColors foreground, CharColors background) {
  char *currentMsg = msg;
  do {
    if (*currentMsg == NULL) {
      break;
    }

    *current_position++ = *currentMsg++;
    *current_position++ = get_char_attr(foreground, background);
  } while (*currentMsg != NULL);
}

void clear_screen(CharColors foreground, CharColors background) {
  reset_current_position();
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLUMNS; j++) {
      *current_position++ = ' ';
      *current_position++ = get_char_attr(foreground, background);
    }
  }
  reset_current_position();
}