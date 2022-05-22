Zoom    sw    0    3    Stack
        lw    0    5    Stack
loop    add    1    1    1
        nor    1    2    2
        beq    2    2    Ziggy
        beq    0    1    exit
exit    halt
Cooki   .fill    3
Ziggy   .fill    loop
Big     .fill    666666
