#ifndef __PIC_H
#define __PIC_H

#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET 0x28

void install_pic(int, int);
void pic_send_eoi(int);

#endif