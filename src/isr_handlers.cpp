#include "include/exceptions.h"

void (*isr_handlers[])(int) = {
    &division_error,      &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &invalid_opcode,      &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler, &default_isr_handler,
    &default_isr_handler, &default_isr_handler};