#ifndef __IDT_H
#define __IDT_H

#include <cstdint>

#include "gdt.h"
// #include "util.h"

#define PRESENT_BIT (1 << 7)
#define DPL_RING_0 0x0

#define TOTAL_ISR 256

enum GATE_TYPE {
  INVALID_0,
  INVALID_1,
  INVALID_2,
  INVALID_3,
  INVALID_4,
  TASK_GATE,
  INT_GATE_16BIT,
  TRAP_GATE_16BIT,
  INVALID_8,
  INVALID_9,
  INVALID_A,
  INVALID_B,
  INVALID_C,
  INVALID_D,
  INT_GATE_32BIT,
  TRAP_GATE_32BIT
};

class IDT {
  uint16_t offset_bytes_0_1;
  uint16_t segment_selector_bytes_2_3{TYPE_CODE_SEG};
  uint8_t reserved_byte_4{0};
  uint8_t type_dpl_present_byte_5{PRESENT_BIT | ((DPL_RING_0 & 0x3) << 5) |
                                  INT_GATE_32BIT};
  uint16_t offset_bytes_6_7;

public:
  void install_idt_entry(void (*isr_entrypoint)(void)) {
    this->offset_bytes_0_1 = (uint32_t)*isr_entrypoint & 0x0000ffff;
    this->offset_bytes_6_7 = (uint32_t)*isr_entrypoint >> 16;
    /*("%b %b %b %b %b\n", this->offset_bytes_6_7,
           this->type_dpl_present_byte_5, this->reserved_byte_4,
           this->segment_selector_bytes_2_3, this->offset_bytes_0_1);*/
  };
} __attribute__((__packed__));

class IDTPointer {
public:
  uint16_t size{0};
  uint32_t entries_address{0};

  IDTPointer(uint16_t size, int32_t address)
      : size(size), entries_address(address) {}
} __attribute__((__packed__));

void install_idt();

#endif