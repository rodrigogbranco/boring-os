#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

void default_isr_handler(int, int);

void division_error(int, int);

void invalid_opcode(int, int);

void general_protection_fault(int, int);

#endif