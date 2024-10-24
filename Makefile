SRCDIR=src
OUTPUTDIR=build

all: bootloader

bootloader.o: $(SRCDIR)/bootloader.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

bootloader: bootloader.o
	ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x0 -s --oformat binary -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

clean:
	rm -rf $(OUTPUTDIR)/*