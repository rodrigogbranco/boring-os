all: bootloader

bootloader.o: bootloader.asm
	nasm -wall -O2 -f elf32 -F dwarf -g bootloader.asm

bootloader: bootloader.o
	ld -nostdlib -g -m elf_i386 -Ttext 0x0 --oformat binary -s -o bootloader bootloader.o

clean:
	rm -rf *.o bootloader