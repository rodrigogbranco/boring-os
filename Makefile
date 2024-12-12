SRCDIR=src
OUTPUTDIR=build

OBJS:=screen.o gdt.o util.o queue.o

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell g++ -m32 $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell g++ -m32 $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: $(OUTPUTDIR) kernel bootloader createimage
	./$(OUTPUTDIR)/createimage --extended ./$(OUTPUTDIR)/bootloader ./$(OUTPUTDIR)/kernel

$(OUTPUTDIR):
	mkdir $@

%.o: $(SRCDIR)/%.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

%.o: $(SRCDIR)/%.cpp
	g++ -g -m32 -c $< -o $(OUTPUTDIR)/$@ -O2 -Wall -Wextra -ffreestanding -fno-rtti -nostdlib -fno-exceptions -fno-use-cxa-atexit 

bootloader: bootloader.o
	ld -O2 -g -m elf_i386 -Ttext 0x0 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

kernel:  kernel.o $(OBJ_LINK_LIST)
	cd $(OUTPUTDIR) && ld -O2 -g -m elf_i386 -T../linker.ld -z noexecstack -o $@ kernel.o $(OBJ_LINK_LIST)

createimage: $(SRCDIR)/createimage.cpp
	g++ -o $(OUTPUTDIR)/$@ $<

clean:
	rm -rf $(OUTPUTDIR)/*