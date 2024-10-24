%define STACK_SEG_BOOTLOADER 0x9000
%define STACK_POINTER_BOOTLOADER 0xffff
%define SEGMENTS_BOOTLOADER 0x7c0

%define BIOS_INT_10H 0x10
%define BIOS_INT_10H_OUTPUT 0x0e
%define BIOS_INT_10H_PAGE_NUMBER_0 0x00
%define BIOS_INT_10H_FOREGROUND_COLOR_GM 0x02

%define NULL_CHARACTER 0

%define LINE_FEED 10
%define CARRIER_RETURN 13

%define BOOT_SIGNATURE db 0x55, 0xaa