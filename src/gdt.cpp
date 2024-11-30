#include "include/gdt.h"

GDT gdt_entries[] = {
    // Null descriptor
    GDT(0,0,0,0),
    // Kernel Code Descriptor
    GDT(0x00000000, 0xfffff,
      GDT_ACCESS_CODE_SEG_READ | GDT_TYPE_CODE_SEG_READ |
          GDT_TYPE_CODE_SEG_NOT_CONFIRMING | GDT_TYPE_CODE_SEG |
          GDT_TYPE_DESCRIPTOR_CODE_OR_DATA | GDT_RING_00 | GDT_TYPE_PRESENT,
      GDT_FLAG_USER_DEFINED | GDT_LONG_MODE_DISABLED | GDT_32BIT_FLAG |
          GDT_GRANULARITY_FLAG),
    // Kernel Data Descriptor          
    GDT(0x00000000, 0xfffff,
      GDT_ACCESS_DATA_SEG_WRITE | GDT_TYPE_DATA_SEG_WRITE |
          GDT_TYPE_DATA_SEG_GROW_UPSIDE | GDT_TYPE_DATA_SEG |
          GDT_TYPE_DESCRIPTOR_CODE_OR_DATA | GDT_RING_00 | GDT_TYPE_PRESENT,
      GDT_FLAG_USER_DEFINED | GDT_LONG_MODE_DISABLED | GDT_32BIT_FLAG |
          GDT_GRANULARITY_FLAG)
};
GDTPointer gdtp((uint16_t)(sizeof(GDT) * GDT_ENTRIES),
                (uint32_t)&gdt_entries[0]);

void install_gdt() {
  GDTPointer testing(0, 0);
    
  asm("lgdt %0" : : "m"(gdtp));
  asm("ljmp %0, $continue_load_gdt_register \n\t"
      "continue_load_gdt_register: \n\t" ::"i"(GDT_KCS_SEL * 0x8));
  asm("mov %0, %%eax \n\t"
      "mov %%ax, %%ds \n\t"
      "mov %%ax, %%es \n\t"
      "mov %%ax, %%fs \n\t"
      "mov %%ax, %%gs \n\t"
      "mov %%ax, %%ss \n\t" ::"i"(GDT_KDS_SEL * 0x8));
}