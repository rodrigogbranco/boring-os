global scheduler_entry
extern current_task, scheduler

scheduler_entry:
    push ebp
    mov ebp, esp
    mov esp, [current_task]
    lea esp, [esp + (9*4)]
    pushfd
    pushad
    mov [esp + (3*4)], ebp
    mov esp, ebp
    call scheduler
    mov esp, [current_task]
    popad
    popfd
    mov esp, [esp - (6*4)]
    pop ebp
    ret




