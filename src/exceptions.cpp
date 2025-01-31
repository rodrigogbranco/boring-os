#include "include/exceptions.h"
#include "include/util.h"

void default_isr_handler(int err_code) {
  printk("default_isr_handler was called: err_code: %d\n", err_code);
}

void division_error(__attribute__((unused)) int err_code) {
  panic("Divisior error");
}

void invalid_opcode(__attribute__((unused)) int err_code) {
  panic("Invalid Opcode");
}