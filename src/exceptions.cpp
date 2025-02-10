#include "include/exceptions.h"
#include "include/kernel.h"
#include "include/util.h"

void default_isr_handler(int isr_nr, int err_code) {
  printk("default_isr_handler was called: isr_nr: %x, err_code: %d\n", isr_nr,
         err_code);
}

void division_error(__attribute__((unused)) int isr_nr,
                    __attribute__((unused)) int err_code) {
  printk("Divisior error, killing\n");
  do_exit();
}

void invalid_opcode(__attribute__((unused)) int isr_nr,
                    __attribute__((unused)) int err_code) {
  printk("Invalid Opcode, killing\n");
  do_exit();
}

void general_protection_fault(__attribute__((unused)) int isr_nr,
                              __attribute__((unused)) int err_code) {
  panic("GPR");
}