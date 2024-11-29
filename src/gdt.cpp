#include "include/gdt.h"
#include "include/screen.h"

GDT gdt_entries[GDT_ENTRIES];
GDTPointer gdtp((uint16_t)(sizeof(GDT) * GDT_ENTRIES),
                (uint32_t)&gdt_entries[0]);

void GDT::install_gdt_entry(uint32_t base_address, uint32_t limit, uint8_t type,
                            uint8_t flags) {
  this->limit_bytes_0_1 = limit & 0xffff;
  this->base_address_bytes_2_3 = base_address & 0xffff;
  this->base_address_byte_4 = (base_address >> 16) & 0xff;
  this->type_access_byte_5 = type;
  this->limit_flags_byte_6 = (limit >> 16) & 0xf;
  this->limit_flags_byte_6 |= flags;
  this->base_address_byte_7 = (base_address >> 24) & 0xff;
}

void install_gdt() {
  // NULL descriptor (gdt_entries[GDT_NULL_SEL]) is constructed by default

  clear_screen(GREEN, BLUE);

  printk((char *)"chegou aqui", GREEN, BLUE);

  // Kernel Code segment
  gdt_entries[GDT_KCS_SEL].install_gdt_entry(
      0x00000000, 0xfffff,
      GDT_ACCESS_CODE_SEG_READ | GDT_TYPE_CODE_SEG_READ |
          GDT_TYPE_CODE_SEG_NOT_CONFIRMING | GDT_TYPE_CODE_SEG |
          GDT_TYPE_DESCRIPTOR_CODE_OR_DATA | GDT_RING_00 | GDT_TYPE_PRESENT,
      GDT_FLAG_USER_DEFINED | GDT_LONG_MODE_DISABLED | GDT_32BIT_FLAG |
          GDT_GRANULARITY_FLAG);
  // Kernel Data segment
  gdt_entries[GDT_KDS_SEL].install_gdt_entry(
      0x00000000, 0xfffff,
      GDT_ACCESS_DATA_SEG_WRITE | GDT_TYPE_DATA_SEG_WRITE |
          GDT_TYPE_DATA_SEG_GROW_UPSIDE | GDT_TYPE_DATA_SEG |
          GDT_TYPE_DESCRIPTOR_CODE_OR_DATA | GDT_RING_00 | GDT_TYPE_PRESENT,
      GDT_FLAG_USER_DEFINED | GDT_LONG_MODE_DISABLED | GDT_32BIT_FLAG |
          GDT_GRANULARITY_FLAG);

  // User code, data segments and TSS goes here

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