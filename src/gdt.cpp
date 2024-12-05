#include "include/gdt.h"

static GDT::GDT entries[] = {
    // Null descriptor
    GDT::GDT(0, 0, 0, 0),
    // Kernel Code Descriptor
    GDT::GDT(0x00000000, 0xfffff,
             ACCESS_CODE_SEG_READ | TYPE_CODE_SEG_READ |
                 TYPE_CODE_SEG_NOT_CONFIRMING | TYPE_CODE_SEG |
                 TYPE_DESCRIPTOR_CODE_OR_DATA | RING_00 | TYPE_PRESENT,
             FLAG_USER_DEFINED | FLAG_LONG_MODE_DISABLED | FLAG_32BIT |
                 FLAG_GRANULARITY),
    // Kernel Data Descriptor
    GDT::GDT(0x00000000, 0xfffff,
             ACCESS_DATA_SEG_WRITE | TYPE_DATA_SEG_WRITE |
                 TYPE_DATA_SEG_GROW_UPSIDE | TYPE_DATA_SEG |
                 TYPE_DESCRIPTOR_CODE_OR_DATA | RING_00 | TYPE_PRESENT,
             FLAG_USER_DEFINED | FLAG_LONG_MODE_DISABLED | FLAG_32BIT |
                 FLAG_GRANULARITY)};
static GDT::GDTPointer gdtp((uint16_t)(sizeof(GDT::GDT) * ENTRIES),
                            (uint32_t)&entries[0]);

void GDT::install_gdt() {
  asm("lgdt %0" : : "m"(gdtp));
  asm("ljmp %0, $continue_load_register \n\t"
      "continue_load_register: \n\t" ::"i"(KCS_SEL));
  asm("mov %0, %%eax \n\t"
      "mov %%ax, %%ds \n\t"
      "mov %%ax, %%es \n\t"
      "mov %%ax, %%fs \n\t"
      "mov %%ax, %%gs \n\t"
      "mov %%ax, %%ss \n\t" ::"i"(KDS_SEL));
}