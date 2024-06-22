#include <stdint.h>
#include <stdalign.h>
#include <stdnoreturn.h>
#include <stddef.h>

#include "idt.h"
#include "utils.h"
#include "pic.h"

#define IDT_DESCRIPTORS_COUNT 	256
#define IRQ_COUNT				16
#define IDT_DESCRIPTOR_FLAG 0x8E
#define ADDR_LOW(A) 	((uint32_t)(A) & 0xFFFF)
#define ADDR_HIGH(A) 	((uint32_t)(A) >> 16)

extern void* isr_stub_table[];

typedef struct IdtEntry {
	uint16_t    isr_low;
	uint16_t    kernel_cs;
	uint8_t     reserved;
	uint8_t     attributes;
	uint16_t    isr_high;
} __attribute__((packed)) IdtEntry;

static alignas(0x10) IdtEntry idt[IDT_DESCRIPTORS_COUNT];
static void (*handlers[IRQ_COUNT])(void) = { 0 };

static char const* const exception_names[] = {
	[0] = "Division Error",
	[1] = "Debug",
	[2] = "Non-maskable interrupt",
	[3] = "Breakpoint",
	[4] = "Overflow",
	[5] = "Bound range exceeded",
	[6] = "Invalid opcode",
	[7] = "Device not available",
	[8] = "Double fault",
	[10] = "Invalid TSS",
	[11] = "Segment not present",
	[12] = "Stack-segment fault",
	[13] = "General protection fault",
	[14] = "Page fault",
	[16] = "x87 Floating-point exception",
	[17] = "Alignment check",
	[18] = "Machine check",
	[19] = "SIMD Floating-point exception",
	[20] = "Virtualization exception",
	[21] = "Control protection exception",
	[30] = "Security exception",
};

noreturn void exception_handler(size_t vector)
{
	char message[256] = "Unhandled exception: ";

	char const* exc_name = exception_names[vector];
	if (!exc_name)
		exc_name = "Unknown exception";

	strcat(message, exc_name);
    panic(message);
}

void irq_handler(size_t irq)
{
	if (handlers[irq])
		handlers[irq]();
	pic_send_eoi(irq);
}

void register_irq_handler(size_t irq, void callback(void))
{
	handlers[irq] = callback;
}

static IdtEntry idt_get_descriptor(void* isr, uint8_t flags)
{
	return (IdtEntry) {
		.isr_low 	= ADDR_LOW(isr),
		.kernel_cs 	= 0x08,
		.attributes = flags,
		.isr_high	= ADDR_HIGH(isr),
		.reserved	= 0
	};
}

void load_idt(uint32_t idt, uint16_t size);

void idt_init(void)
{
	for (size_t vector = 0; vector < 48; vector++)
		idt[vector] = idt_get_descriptor(isr_stub_table[vector], IDT_DESCRIPTOR_FLAG);

	load_idt((uint32_t)&idt, sizeof idt);
}

void disable_interrupts(void)
{
	__asm__ volatile ("cli");
}

void enable_interrupts(void)
{
	__asm__ volatile ("sti");
}
