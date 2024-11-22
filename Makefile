SRCDIR=src
OUTPUTDIR=build

all: $(OUTPUTDIR) kernel bootloader createimage
	./$(OUTPUTDIR)/createimage --extended ./$(OUTPUTDIR)/bootloader ./$(OUTPUTDIR)/kernel

$(OUTPUTDIR):
	mkdir $@

%.o: $(SRCDIR)/%.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

%.o: $(SRCDIR)/%.cpp
	g++ -g -m32 -c $< -o $(OUTPUTDIR)/$@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -nostdlib

bootloader: bootloader.o
	ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x0 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

kernel: kernel.o screen.o
	ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x1000 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/kernel.o $(OUTPUTDIR)/screen.o


createimage: $(SRCDIR)/createimage.cpp
	g++ -o $(OUTPUTDIR)/$@ $<

clean:
	rm -rf $(OUTPUTDIR)/*