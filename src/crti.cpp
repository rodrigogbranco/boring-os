/* adapted from
 * https://wiki.osdev.org/Calling_Global_Constructors#Using_crti.o,_crtbegin.o,_crtend.o,_and_crtn.o_in_a_Kernel
 */

typedef void (*func_ptr)(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern func_ptr _init_array_start[0], _init_array_end[0];
extern func_ptr _fini_array_start[0], _fini_array_end[0];
#pragma GCC diagnostic pop

extern "C" void _init(void) {
  for (func_ptr *func = _init_array_start; func != _init_array_end; func++)
    (*func)();
}

extern "C" void _fini(void) {
  for (func_ptr *func = _fini_array_start; func != _fini_array_end; func++)
    (*func)();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
func_ptr _init_array_start[0] __attribute__((used, section(".init_array"),
                                             aligned(sizeof(func_ptr)))) = {};
func_ptr _fini_array_start[0] __attribute__((used, section(".fini_array"),
                                             aligned(sizeof(func_ptr)))) = {};
#pragma GCC diagnostic pop