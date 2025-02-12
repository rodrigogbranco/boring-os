#include "include/interrupt.h"
#include "include/common.h"
#include "include/pic.h"
#include "include/util.h"

static int teste = 0;

void default_irq_handler(int isr_nr, __attribute__((unused)) int err_code) {
  pic_send_eoi(isr_nr);
  printk("IRQ ISR %x\n", isr_nr);
};

void irq0_system_timer(int isr_nr, __attribute__((unused)) int err_code) {
  pic_send_eoi(isr_nr);
  printk("IRQ0 %d\n", teste++);
  //  delay(20);
};