        lw    0    2    neg1
loop    beq   1    0    exit
        add   1    2    1
        lw    0    4    zoom
        lw    3    2    loop
        sw    4    3    Cooki
skip    lw    4    3    cooki
        beq   0    0    loop
exit    lw    0    1    exit
abc     .fill    skip
zoom    .fill    Zoom
cooki   .fill    Queue
neg1    .fill    -1
