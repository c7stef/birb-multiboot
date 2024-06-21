.PHONY: build iso clean run

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

CC = i686-elf-gcc
AS = i686-elf-as

CFLAGS = -ffreestanding -O3 -Wall -Wextra -pedantic -std=c11
LDFLAGS = -T linker.ld -nostdlib -lgcc

C_SRCS = 			\
	main.c			\
	birb.c			\
	utils.c 		\
	term.c			\
	gdt.c			\
	idt.c			\
	pic.c			\
	keyboard.c		\
	screen.c

AS_SRCS = 			\
	boot.s

NASM_SRCS = 		\
	irq.asm			\
	load_gdt.asm	\
	load_idt.asm	\
	int32.asm

C_OBJS = $(C_SRCS:%.c=%.o)
AS_OBJS = $(AS_SRCS:%.s=%.o)
NASM_OBJS = $(NASM_SRCS:%.asm=%.o)

ISODIR = ./isodir
ISO_BOOTDIR = $(ISODIR)/boot
ISO_GRUBDIR = $(ISO_BOOTDIR)/grub

iso: birb.iso

run: iso
	qemu-system-i386 -cdrom birb.iso

build: birb.bin

birb.iso: birb.bin grub.cfg
	mkdir -p $(ISO_GRUBDIR)
	cp grub.cfg $(ISO_GRUBDIR)
	cp birb.bin $(ISO_BOOTDIR)
	grub-mkrescue -o birb.iso $(ISODIR)

birb.bin: $(C_OBJS) $(AS_OBJS) $(NASM_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o : %.c $(DEPDIR)/%.d | $(DEPDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

%.o : %.s
	$(AS) $< -o $@

%.o : %.asm
	nasm -felf32 $< -o $@

clean:
	rm -rf *.o birb.bin birb.iso

$(DEPDIR):
	@mkdir -p $@

DEPFILES := $(C_SRCS:%.c=$(DEPDIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))
