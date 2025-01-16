global scheduler_entry, kernel_entry
extern current_task, scheduler, syscall_operations, returning_kernel_entry, start_timer, stop_timer

%define ESP_OFFSET (3*4)
%define K_REGS_LIMIT (9*4)
%define U_REGS_LIMIT (18*4)
%define ESP_OFFSET_FROM_LIMIT (6*4)

syscall_op dd 0
old_esp dd 0

kernel_entry:
    push ebp
    mov ebp, esp
    call stop_timer
    mov esp, [current_task]
    lea esp, [esp + U_REGS_LIMIT]
    pushfd
    pushad
    mov [esp + ESP_OFFSET], ebp

    mov ebx, [ebp+8]
    mov [syscall_op], ebx

    mov esp, [current_task]
    popad
    popfd
    mov esp, [esp - ESP_OFFSET_FROM_LIMIT]

    call start_timer
    mov ebx, [syscall_op]
    call [syscall_operations + 4*ebx]

returning_kernel_entry:  
    call stop_timer
    mov [old_esp], esp
    mov esp, [current_task]
    lea esp, [esp + K_REGS_LIMIT]
    pushfd
    pushad
    mov ebx, [old_esp]
    mov [esp + ESP_OFFSET], ebx  

    mov esp, [current_task]
    lea esp, [esp + K_REGS_LIMIT]    
    popad
    popfd
    mov esp, [esp - ESP_OFFSET_FROM_LIMIT]

    call start_timer
    pop ebp
    ret


scheduler_entry:
    push ebp
    mov ebp, esp
    call stop_timer
    mov esp, [current_task]
    lea esp, [esp + K_REGS_LIMIT]
    pushfd
    pushad
    mov [esp + ESP_OFFSET], ebp
    mov esp, ebp
    call scheduler
    mov esp, [current_task]
    popad
    popfd
    mov esp, [esp - ESP_OFFSET_FROM_LIMIT]
    call start_timer
    pop ebp
    ret