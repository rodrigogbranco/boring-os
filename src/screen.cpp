#include "include/screen.h"

static DisplayChar *video_memory = (DisplayChar *)SCREEN_ADDR;

static int display_position = 0;
static char current_color = (GRAY & 0x0f) | (BLACK << 4);

void set_display_colors(DisplayCharColor foreground,
                        DisplayCharColor background) {
  current_color = (foreground & 0x0f) | (background << 4);
}

static inline void reset_current_position() { display_position = 0; }

void print_char(char c) {
  video_memory[display_position].character = c;
  video_memory[display_position].attribute = current_color;
  display_position = (display_position + 1) % (NUM_ROWS * NUM_COLUMNS);
}

void clear_screen() {
  int pos = 0;
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLUMNS; j++) {
      video_memory[pos].character = ' ';
      video_memory[pos++].attribute = current_color;
    }
  }
  reset_current_position();
}

void set_pos(int x, int y) {
  display_position = (((x * NUM_COLUMNS) + y) % (NUM_ROWS * NUM_COLUMNS));
}

void line_feed() {
  display_position += NUM_COLUMNS;
  display_position -= display_position % NUM_COLUMNS;
  display_position = display_position % (NUM_ROWS * NUM_COLUMNS);
}

void carriage_return() {
  display_position -= display_position % NUM_COLUMNS;
  display_position = display_position % (NUM_ROWS * NUM_COLUMNS);
}