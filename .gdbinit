define freertos_task_backtrace

    # Register stored by CPU when exception
    set $sp=&((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[17]
    set $xpsr=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[16]
    set $pc=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[15]
    set $lr=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[14]
    set $r12=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[13]
    set $r3=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[12]
    set $r2=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[11]
    set $r1=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[10]
    set $r0=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[9]

    # Registers stored when context switch
    set $lr=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[8]
    set $r11=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[7]
    set $r10=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[6]
    set $r9=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[5]
    set $r8=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[4]
    set $r7=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[3]
    set $r6=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[2]
    set $r5=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[1]
    set $r4=((uint32_t*)((TCB_t*)($arg0))->pxTopOfStack)[0]

    bt
end