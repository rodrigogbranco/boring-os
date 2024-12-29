#include "include/util.h"
#include "include/screen.h"

#include <cstdarg>

char *Util::itoa(int number, int radix, char *tmpBuff) {
  int i = 0;
  int j = 0;
  char tmp[100];
  if (number < 0) {
    tmpBuff[i++] = '-';
    number *= -1;
  }
  if (radix == 8 || radix == 16) {
    tmpBuff[i++] = '0';
  }
  if (radix == 16) {
    tmpBuff[i++] = 'x';
  }

  do {
    char c = number % radix;
    if (radix == 16 && c >= 10) {
      c = c + 'A' - 10;
    } else {
      c = c + '0';
    }
    tmp[j++] = c;
    number /= radix;
  } while (number > 0);

  while (--j >= 0) {
    tmpBuff[i++] = tmp[j];
  }
  if (radix == 2) {
    tmpBuff[i++] = 'b';
  }

  tmpBuff[i++] = '\0';
  return tmpBuff;
}

void Util::printk(const char *fmt, ...) {
  std::va_list args;
  int n = 0;
  char tmp = 0;
  char tmpBuff[100];
  va_start(args, fmt);
  for (const char *c = fmt; *c != '\0'; ++c) {
    switch (*c) {
    case '%':
      switch (*++c) {
      case 's':
        for (const char *s = va_arg(args, const char *); *s != '\0'; ++s) {
          Screen::print_char(*s);
        }
        break;
      case 'd':
        n = va_arg(args, int);
        for (const char *s = Util::itoa(n, 10, tmpBuff); *s != '\0'; ++s) {
          Screen::print_char(*s);
        }
        break;
      case 'x':
        n = va_arg(args, int);
        for (const char *s = Util::itoa(n, 16, tmpBuff); *s != '\0'; ++s) {
          Screen::print_char(*s);
        }
        break;
      case 'b':
        n = va_arg(args, int);
        for (const char *s = Util::itoa(n, 2, tmpBuff); *s != '\0'; ++s) {
          Screen::print_char(*s);
        }
        break;
      case 'o':
        n = va_arg(args, int);
        for (const char *s = Util::itoa(n, 8, tmpBuff); *s != '\0'; ++s) {
          Screen::print_char(*s);
        }
        break;
      case 'c':
        tmp = va_arg(args, int);
        Screen::print_char(tmp);
        break;
      case '%':
        Screen::print_char('%');
        break;
      }
      break;
    case '\n':
      Screen::line_feed();
      break;
    case '\t':
      for (int i = 0; i < 4; i++) {
        Screen::print_char(' ');
      }
      break;
    default:
      Screen::print_char(*c);
      break;
    }
  }
  va_end(args);
}

void Util::panic(const char *fmt) {
  Screen::set_colors(Screen::RED, Screen::WHITE);
  // Screen::clear_screen();
  Util::printk("PANIC: %s\n", fmt);
  while (true) {
    /* BUSY LOOP*/
  }
}