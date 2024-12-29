#include "include/screen.h"
#include "include/kernel.h"

extern Datastructure::QueueNode<PCB> *current_task;

static Screen::Char *video_memory = (Screen::Char *)SCREEN_ADDR;

void Screen::set_colors(Screen::Colors foreground, Screen::Colors background) {
  current_task->get().set_display_color(foreground, background);
}

static inline void reset_current_position() {
  current_task->get().set_display_position(0);
}

void Screen::print_char(char c) {
  int pos = current_task->get().get_display_position();
  video_memory[pos].character = c;
  video_memory[pos].attribute = current_task->get().get_display_color();
  current_task->get().set_display_position((pos + 1) %
                                           (NUM_ROWS * NUM_COLUMNS));
}

void Screen::clear_screen() {
  int pos = 0;
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLUMNS; j++) {
      video_memory[pos].character = ' ';
      video_memory[pos++].attribute = current_task->get().get_display_color();
    }
  }
  reset_current_position();
}

void Screen::set_pos(int x, int y) {
  current_task->get().set_display_position(
      (((x * NUM_COLUMNS) + y) % (NUM_ROWS * NUM_COLUMNS)));
}

void Screen::line_feed() {
  int pos = current_task->get().get_display_position();
  pos += NUM_COLUMNS;
  pos -= pos % NUM_COLUMNS;
  pos = pos % (NUM_ROWS * NUM_COLUMNS);
  current_task->get().set_display_position(pos);
}

void Screen::carriage_return() {
  int pos = current_task->get().get_display_position();
  pos -= pos % NUM_COLUMNS;
  pos = pos % (NUM_ROWS * NUM_COLUMNS);
  current_task->get().set_display_position(pos);
}