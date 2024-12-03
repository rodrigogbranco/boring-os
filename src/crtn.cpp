/* get from
 * https://wiki.osdev.org/Calling_Global_Constructors#Using_crti.o,_crtbegin.o,_crtend.o,_and_crtn.o_in_a_Kernel
 */
typedef void (*func_ptr)(void);

func_ptr _init_array_end[0] __attribute__((used, section(".init_array"),
                                           aligned(sizeof(func_ptr)))) = {};
func_ptr _fini_array_end[0] __attribute__((used, section(".fini_array"),
                                           aligned(sizeof(func_ptr)))) = {};