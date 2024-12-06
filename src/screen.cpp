#include "include/screen.h"

static Screen::Char *video_memory = (Screen::Char *)SCREEN_ADDR;
static int current_position = 0;

static inline char get_char_attr(Screen::Colors foreground,
                                 Screen::Colors background) {
  return (foreground & 0x0f) | (background << 4);
}

char current_color = get_char_attr(Screen::GRAY, Screen::BLACK);

void Screen::set_colors(Screen::Colors foreground, Screen::Colors background) {
  current_color = get_char_attr(foreground, background);
}

static inline void reset_current_position() { current_position = 0; }

void Screen::print_char(char c) {
  video_memory[current_position].character = c;
  video_memory[current_position].attribute = current_color;
  current_position = (current_position + 1) % (NUM_ROWS * NUM_COLUMNS);
}

void Screen::clear_screen() {
  reset_current_position();
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLUMNS; j++) {
      video_memory[current_position].character = ' ';
      video_memory[current_position].attribute = current_color;
      current_position = (current_position + 1) % (NUM_ROWS * NUM_COLUMNS);
    }
  }
  reset_current_position();
}

void Screen::set_pos(int x, int y) {
  current_position = (((x * NUM_COLUMNS) + y) % (NUM_ROWS * NUM_COLUMNS));
}

void Screen::line_feed() {
  current_position += NUM_COLUMNS;
  current_position -= current_position % NUM_COLUMNS;
  current_position = current_position % (NUM_ROWS * NUM_COLUMNS);
}