#ifndef __GDT_H

#define __GDT_H

#include <cstdint>

// GDT BIT 40
#define ACCESS_CODE_SEG_READ 0
#define ACCESS_DATA_SEG_WRITE 0

// GDT BIT 41
#define TYPE_CODE_SEG_READ (1 << 1)
#define TYPE_DATA_SEG_WRITE (1 << 1)

// GDT BIT 42
#define TYPE_CODE_SEG_NOT_CONFIRMING (0 << 2)
#define TYPE_DATA_SEG_GROW_UPSIDE (0 << 2)

// GDT BIT 43
#define TYPE_CODE_SEG (1 << 3)
#define TYPE_DATA_SEG (0 << 3)

// GDT BIT 44
#define TYPE_DESCRIPTOR_CODE_OR_DATA (1 << 4)
#define TYPE_DESCRIPTOR_TSS (0 << 4)

// GDT BIT 45 and 46
#define RING_00 (0x0 << 5)
#define RING_01 (0x1 << 5)
#define RING_10 (0x2 << 5)
#define RING_11 (0x3 << 5)

// GDT BIT 47
#define TYPE_PRESENT (1 << 7)

// GDT BIT 52
#define FLAG_USER_DEFINED (0 << 4)

// GDT BIT 53
#define FLAG_LONG_MODE_DISABLED (0 << 5)

// GDT BIT 54
#define FLAG_32BIT (1 << 6)

// GDT BIT 55
#define FLAG_GRANULARITY (1 << 7)

class GDT {
public:
  uint16_t limit_bytes_0_1 = 0;
  uint16_t base_address_bytes_2_3 = 0;
  uint8_t base_address_byte_4 = 0;
  uint8_t type_access_byte_5 = 0;
  uint8_t limit_flags_byte_6 = 0;
  uint8_t base_address_byte_7 = 0;

  GDT(uint32_t base_address, uint32_t limit, uint8_t type, uint8_t flags)
      : limit_bytes_0_1(limit & 0xffff),
        base_address_bytes_2_3(base_address & 0xffff),
        base_address_byte_4((base_address >> 16) & 0xff),
        type_access_byte_5(type),
        limit_flags_byte_6(((limit >> 16) & 0xf) | flags),
        base_address_byte_7((base_address >> 24) & 0xff) {};
} __attribute__((__packed__));

class GDTPointer {
public:
  uint16_t size = 0;
  uint32_t entries_address = 0;

  GDTPointer(uint16_t size, uint32_t address)
      : size(size), entries_address(address) {}
} __attribute__((__packed__));

void install_gdt();

#endif