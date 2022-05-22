        lw      3       4       exit
        lw      2       3       6
        jalr    2       2
        add     4       0       1
loopy   beq     0       0      Zoom
Zoom    noop
        add     0       3       0
        beq     1       5       loopy
        nor     3       4       7
        nor     3       7       1
exit    halt
        noop
