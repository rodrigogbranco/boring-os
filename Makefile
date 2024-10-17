all: bootloader

bootloader.o: bootloader.asm
	nasm -wall -O2 -g -f elf32 bootloader.asm

bootloader: bootloader.o
	ld -nostdlib -m elf_i386 -Ttext 0x0 --oformat binary -s -o bootloader bootloader.o

clean:
	rm -rf *.o bootloader