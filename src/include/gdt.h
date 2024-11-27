#ifndef __GDT_H

#define __GDT_H

#include <stdint.h>

#define GDT_ENTRIES 3
#define GDT_KCS_SEL 1
#define GDT_KDS_SEL 2

// GDT BIT 40
#define GDT_ACCESS_CODE_SEG_READ 0
#define GDT_ACCESS_DATA_SEG_WRITE 0

// GDT BIT 41
#define GDT_TYPE_CODE_SEG_READ (1 << 1)
#define GDT_TYPE_DATA_SEG_WRITE (1 << 1)

// GDT BIT 42
#define GDT_TYPE_CODE_SEG_NOT_CONFIRMING (0 << 2)
#define GDT_TYPE_DATA_SEG_GROW_UPSIDE (0 << 2)

// GDT BIT 43
#define GDT_TYPE_CODE_SEG (1 << 3)
#define GDT_TYPE_DATA_SEG (0 << 3)

// GDT BIT 44
#define GDT_TYPE_DESCRIPTOR_CODE_OR_DATA (1 << 4)
#define GDT_TYPE_DESCRIPTOR_TSS (0 << 4)

// GDT BIT 45 and 46
#define GDT_RING_00 (0x0 << 5)
#define GDT_RING_01 (0x1 << 5)
#define GDT_RING_10 (0x2 << 5)
#define GDT_RING_11 (0x3 << 5)

// GDT BIT 47
#define GDT_TYPE_PRESENT (1 << 7)

// GDT BIT 52
#define GDT_FLAG_USER_DEFINED (0 << 4)

// GDT BIT 53
#define GDT_LONG_MODE_DISABLED (0 << 5)

// GDT BIT 54
#define GDT_32BIT_FLAG (1 << 6)

// GDT BIT 55
#define GDT_GRANULARITY_FLAG (1 << 7)

class GDT {
public:
  uint16_t limit_bytes_0_1 = 0;
  uint16_t base_address_bytes_2_3 = 0;
  uint8_t base_address_byte_4 = 0;
  uint8_t type_access_byte_5 = 0;
  uint8_t limit_flags_byte_6 = 0;
  uint8_t base_address_byte_7 = 0;

  void install_gdt_entry(uint32_t, uint32_t, uint8_t, uint8_t);
} __attribute__((__packed__));

class GDT_Pointer {
public:
  uint16_t size;
  uint32_t gdt_entries_address;
} __attribute__((__packed__));

void install_gdt_entries();
void load_gdt_register();

#endif