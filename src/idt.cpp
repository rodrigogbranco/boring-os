#include "include/idt.h"
#include "include/exceptions.h"

static IDT entries[256];
static IDTPointer idtp((uint16_t)(sizeof(entries) - 1), (uint32_t)&entries[0]);

extern "C" void (*isr[])(void);
extern "C" uint32_t isr_size;
extern "C" void default_isr(void);

void install_idt() {
  unsigned int i = 0;
  while (i < isr_size) {
    entries[i].install_idt_entry(isr[i],
                                 i < 32 ? TRAP_GATE_32BIT : INT_GATE_32BIT);
    i++;
  }

  while (i < TOTAL_ISR) {
    entries[i].install_idt_entry(&default_isr, INT_GATE_32BIT);
    i++;
  }

  asm("lidt %0" : : "m"(idtp));
}