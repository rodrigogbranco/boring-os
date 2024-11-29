SRCDIR=src
OUTPUTDIR=build

OBJS:=screen.o gdt.o

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CC) -m32 $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) -m32 $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: $(OUTPUTDIR) kernel bootloader createimage
	./$(OUTPUTDIR)/createimage --extended ./$(OUTPUTDIR)/bootloader ./$(OUTPUTDIR)/kernel

$(OUTPUTDIR):
	mkdir $@

%.o: $(SRCDIR)/%.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

%.o: $(SRCDIR)/%.cpp
	$(CC) -g -m32 -c $< -o $(OUTPUTDIR)/$@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -nostdlib -fno-rtti -nodefaultlibs -nostartfiles

bootloader: bootloader.o
	ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x0 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

kernel:  kernel.o $(OBJ_LINK_LIST)
	cd $(OUTPUTDIR) && ld -nostdlib -O2 -g -m elf_i386 -Ttext 0x1000 --section-start .init=0x5000 -z noexecstack -o $@ kernel.o $(OBJ_LINK_LIST)

createimage: $(SRCDIR)/createimage.cpp
	g++ -o $(OUTPUTDIR)/$@ $<

clean:
	rm -rf $(OUTPUTDIR)/*