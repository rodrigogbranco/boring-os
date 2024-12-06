#include "include/gdt.h"
#include "include/screen.h"
#include "include/util.h"

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void _start() {
  _init();
  GDT::install_gdt();

  char test[] = {'t', 'e', 's', 't', '\0'};

  Screen::set_colors(Screen::RED, Screen::YELLOW);
  Screen::clear_screen();
  Util::printk("Hello %s %d!\n", "Rodrigo", -42);
  Util::printk("\tTesting line feed! %c %x %o\n%s", 'X', 0xb8000, 0700, test);

  while (true) {
    /* BUSY LOOP*/
  }

  /*If it reaches here, something very wrong happened...*/
  _fini();
}