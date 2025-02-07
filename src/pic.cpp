#include "include/pic.h"
#include "include/common.h"

#include <cstdint>

#define PIC_MASTER_IO_COMMAND_PORT 0x20
#define PIC_SLAVE_IO_COMMAND_PORT 0xA0

#define ICW1_INIT 1 << 4

#define ICW1_WITH_ICW4 1 << 0
#define ICW1_WITHOUT_ICW4 0 << 0

#define ICW1_SINGLE_MODE 1 << 1
#define ICW1_CASCADE_MODE 0 << 1

#define ICW1_INTERVAL_4 1 << 2
#define ICW1_INTERVAL_8 0 << 2

#define ICW1_EDGE_MODE 1 << 3
#define ICW1_LEVEL_MODE 0 << 3

#define ICW3_MASTER_IRQ2 1 << 2
#define ICW3_SLAVE_IRQ2_BINARY 2

#define ICW4_8085 0
#define ICW4_8086 1

#define ICW4_NORMAL_EOI 0 << 1
#define ICW4_AUTO_EOI 1 << 1

#define ICW4_NON_BUFFERED 0b00 << 3
#define ICW4_BUFFERED_SLAVE 0b10 << 3
#define ICW4_BUFFERED_MASTER 0b11 << 3

#define ICW4_NOT_FULLY_NESTED_MODE 0 << 4
#define ICW4_FULLY_NESTED_MODE 1 << 4

#define PIC_EOI 0x20
#define DISABLE_ALL_INTERRUPTS_MASK 0xff

void install_pic(int new_offset_pic_master, int new_offset_pic_slave) {
  // ICW1 (Initialization Command Words number one - Init command)
  outb(PIC_MASTER_IO_COMMAND_PORT, ICW1_INIT | ICW1_WITH_ICW4 |
                                       ICW1_CASCADE_MODE | ICW1_INTERVAL_8 |
                                       ICW1_LEVEL_MODE);
  outb(PIC_SLAVE_IO_COMMAND_PORT, ICW1_INIT | ICW1_WITH_ICW4 |
                                      ICW1_CASCADE_MODE | ICW1_INTERVAL_8 |
                                      ICW1_LEVEL_MODE);
  io_wait();

  // ICW2 (Offset definition)
  outb(PIC_MASTER_IO_DATA_PORT, new_offset_pic_master);
  outb(PIC_SLAVE_IO_DATA_PORT, new_offset_pic_slave);
  io_wait();

  // ICW3 (Cascade mode)
  outb(PIC_MASTER_IO_DATA_PORT, ICW3_MASTER_IRQ2);
  outb(PIC_SLAVE_IO_DATA_PORT, ICW3_SLAVE_IRQ2_BINARY);
  io_wait();

  // ICW4 (Operation Model)
  outb(PIC_MASTER_IO_DATA_PORT, ICW4_8086 | ICW4_NORMAL_EOI |
                                    ICW4_NON_BUFFERED |
                                    ICW4_NOT_FULLY_NESTED_MODE);
  outb(PIC_SLAVE_IO_DATA_PORT, ICW4_8086 | ICW4_NORMAL_EOI | ICW4_NON_BUFFERED |
                                   ICW4_NOT_FULLY_NESTED_MODE);
  io_wait();

  outb(PIC_MASTER_IO_DATA_PORT, DISABLE_ALL_INTERRUPTS_MASK);
  outb(PIC_SLAVE_IO_DATA_PORT, DISABLE_ALL_INTERRUPTS_MASK);

  sti();
}

void pic_send_eoi(int isr_nr) {
  if (isr_nr >= PIC_SLAVE_OFFSET) {
    outb(PIC_SLAVE_IO_COMMAND_PORT, PIC_EOI);
  }

  outb(PIC_MASTER_IO_COMMAND_PORT, PIC_EOI);
}