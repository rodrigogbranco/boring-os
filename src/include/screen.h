#ifndef __SCREEN_H

#define __SCREEN_H

#define NULL 0
#define SCREEN_ADDR 0xb8000
#define NUM_ROWS 25
#define NUM_COLUMNS 80

namespace Screen {
enum Colors {
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

class Char {
public:
  char character;
  char attribute;
};

void set_colors(Colors, Colors);
void clear_screen();
void set_pos(int, int);
void printk(char *);
void printk(const char *);
void print_char(char);
void line_feed();
} // namespace Screen

#endif