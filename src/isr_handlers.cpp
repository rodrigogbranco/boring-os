#include "include/exceptions.h"
#include "include/interrupt.h"

void (*isr_handlers[])(int, int) = {
    // exceptions
    &division_error, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &invalid_opcode, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler,
    // IRQS
    &irq0_system_timer, &default_irq_handler, &default_irq_handler,
    &default_irq_handler, &default_irq_handler, &default_irq_handler,
    &default_irq_handler, &default_irq_handler, &default_irq_handler,
    &default_irq_handler, &default_irq_handler, &default_irq_handler,
    &default_irq_handler, &default_irq_handler, &default_irq_handler,
    &default_irq_handler};