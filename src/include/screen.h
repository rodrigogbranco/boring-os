#ifndef __SCREEN_H

#define __SCREEN_H

#define NULL 0
#define SCREEN_POINTER (const char *)0xb8000
#define CHAR_ATTR_BLACK_WHITE 0x0f
#define CHAR_ATTR_RED_ 0x40
#define NUM_ROWS 80
#define NUM_COLUMNS 25

enum CharColors {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  PURPLE,
  BROWN,
  GRAY,
  DARK_GRAY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_PURPLE,
  YELLOW,
  WHITE
};

void clear_screen(CharColors, CharColors);
void printk(char *, CharColors, CharColors);

#endif