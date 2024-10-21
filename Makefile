all: bootloader

bootloader.o: bootloader.asm
	nasm -wall -O2 -f elf32 -F dwarf -g $<

bootloader: bootloader.o
	ld -nostdlib -g -m elf_i386 -Ttext 0x0 --oformat binary -s -o $@ $<

clean:
	rm -rf *.o bootloader