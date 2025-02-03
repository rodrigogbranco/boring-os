SRCDIR=src
OUTPUTDIR=build

KOBJS:=screen.o gdt.o util.o tasks.o scheduler.o entry.o queue-impl-pcb.o lock.o isr.o idt.o exceptions.o isr_handlers.o
UOBJS:=syslib.o util.o screen.o

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell g++ -m32 $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell g++ -m32 $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

KOBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(KOBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
UOBJ_LINK_LIST_BEG:=crt0.o  $(CRTI_OBJ) $(CRTBEGIN_OBJ) 
UOBJ_LINK_LIST_END:=$(UOBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: $(OUTPUTDIR) kernel bootloader createimage process1 process2
	cd $(OUTPUTDIR) && ./createimage --extended ./bootloader ./kernel ./process1 ./process2

$(OUTPUTDIR):
	mkdir $@

%.o: $(SRCDIR)/%.asm
	nasm -wall -O2 -f elf32 -F dwarf -g -o $(OUTPUTDIR)/$@ $<

%.o: $(SRCDIR)/%.cpp
	g++ -g -m32 -c $< -o $(OUTPUTDIR)/$@ -O2 -Wall -Wextra -pedantic -Weffc++ -ffreestanding -fno-rtti -nostdlib -fno-exceptions -fno-use-cxa-atexit 

bootloader: bootloader.o
	ld -O2 -g -m elf_i386 -Ttext 0x0 -o $(OUTPUTDIR)/$@ $(OUTPUTDIR)/$<

kernel:  kernel.o $(KOBJ_LINK_LIST)
	cd $(OUTPUTDIR) && ld -O2 -g -m elf_i386 -T../linker.ld -Ttext 0x1000 -z noexecstack -o $@ kernel.o $(KOBJ_LINK_LIST)

process1: $(UOBJ_LINK_LIST_BEG) process1.o $(UOBJ_LINK_LIST_END)
	cd $(OUTPUTDIR) && ld -O2 -g -m elf_i386 -T../linker.ld -Ttext 0x10000 -z noexecstack -o $@ $(UOBJ_LINK_LIST_BEG) process1.o $(UOBJ_LINK_LIST_END)

process2: $(UOBJ_LINK_LIST_BEG) process2.o $(UOBJ_LINK_LIST_END)
	cd $(OUTPUTDIR) && ld -O2 -g -m elf_i386 -T../linker.ld -Ttext 0x20000 -z noexecstack -o $@ $(UOBJ_LINK_LIST_BEG) process2.o $(UOBJ_LINK_LIST_END)	

createimage: $(SRCDIR)/createimage.cpp
	g++ -Wall -Wextra -pedantic -Weffc++ -o $(OUTPUTDIR)/$@ $<

clean:
	rm -rf $(OUTPUTDIR)/*