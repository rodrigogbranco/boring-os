#include "include/pit.h"
#include "include/common.h"

#define FREQUENCY_DIVISOR 1193182
#define UNMASK_IRQ_0 0xfe

#define MODE_COMMAND_PORT 0x43
#define CHANNEL_0_DATA_PORT 0x40

#define BINARY_MODE 0b0
#define RATE_GENERATOR 0b010 << 1
#define LOW_BYTE_HIGH_BYTE 0b11 << 4
#define COUNTER_0 0b00 << 6

void install_pit(int desired_hz) {
  unsigned int divisor = FREQUENCY_DIVISOR / desired_hz;

  outb(MODE_COMMAND_PORT,
       BINARY_MODE | RATE_GENERATOR | LOW_BYTE_HIGH_BYTE | COUNTER_0);

  outb(CHANNEL_0_DATA_PORT, divisor & 0xff);

  outb(CHANNEL_0_DATA_PORT, divisor >> 8);

  outb(PIC_MASTER_IO_DATA_PORT, inb(PIC_MASTER_IO_DATA_PORT) & UNMASK_IRQ_0);
}