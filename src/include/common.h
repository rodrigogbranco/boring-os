#ifndef __COMMOM_H
#define __COMMOM_H

/*Adapted from https://wiki.osdev.org/Inline_Assembly/Examples*/

#define PIC_MASTER_IO_DATA_PORT 0x21
#define PIC_SLAVE_IO_DATA_PORT 0xA1

#include <cstdint>

#define UNUSED_PORT 0x80

static inline void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

static inline void io_wait() { outb(UNUSED_PORT, 0); }

static inline void cli() { asm volatile("cli" : :); }

static inline void sti() { asm volatile("sti" : :); }

#endif