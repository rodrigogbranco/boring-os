#include "include/util.h"
#include "include/screen.h"

#include <cstdarg>

char *itoa(int number, int radix, char *tmpBuff) {
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

void printk(const char *fmt, ...) {
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
          print_char(*s);
        }
        break;
      case 'u':
        n = va_arg(args, unsigned int);
        for (const char *s = itoa(n, 10, tmpBuff); *s != '\0'; ++s) {
          print_char(*s);
        }
        break;
      case 'd':
        n = va_arg(args, int);
        for (const char *s = itoa(n, 10, tmpBuff); *s != '\0'; ++s) {
          print_char(*s);
        }
        break;
      case 'x':
        n = va_arg(args, int);
        for (const char *s = itoa(n, 16, tmpBuff); *s != '\0'; ++s) {
          print_char(*s);
        }
        break;
      case 'b':
        n = va_arg(args, int);
        for (const char *s = itoa(n, 2, tmpBuff); *s != '\0'; ++s) {
          print_char(*s);
        }
        break;
      case 'o':
        n = va_arg(args, int);
        for (const char *s = itoa(n, 8, tmpBuff); *s != '\0'; ++s) {
          print_char(*s);
        }
        break;
      case 'c':
        tmp = va_arg(args, int);
        print_char(tmp);
        break;
      case '%':
        print_char('%');
        break;
      }
      break;
    case '\n':
      line_feed();
      break;
    case '\t':
      for (int i = 0; i < 4; i++) {
        print_char(' ');
      }
      break;
    default:
      print_char(*c);
      break;
    }
  }
  va_end(args);
}

void panic(const char *fmt) {
  set_display_colors(RED, WHITE);
  // clear_screen();
  printk("PANIC: %s\n", fmt);
  while (true) {
    /* BUSY LOOP*/
  }
}

void set_display_position(int x, int y) { set_pos(x, y); }