SRCDIR=src
OUTPUTDIR=build

OBJS:=screen.o gdt.o
OBJ_LINK_LIST:=kernel.o $(OBJS)

all: $(OUTPUTDIR) kernel bootloader createimage
	./$(OUTPUTDIR)/createimage --extended ./$(OUTPUTDIR)/bootloader ./$(OUTPUTDIR)/kernel

$(OUTPUTDIR):
	mkdir $@

%.o: $(SRCDIR)/%.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

%.o: $(SRCDIR)/%.cpp
	g++ -g -m32 -c $< -o $(OUTPUTDIR)/$@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -nostdlib -fno-rtti -nostartfiles

bootloader: bootloader.o
	ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x0 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

kernel:  $(OBJ_LINK_LIST)
	cd $(OUTPUTDIR) && ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x1000  -o $@ $(OBJ_LINK_LIST)

createimage: $(SRCDIR)/createimage.cpp
	g++ -o $(OUTPUTDIR)/$@ $<

clean:
	rm -rf $(OUTPUTDIR)/*