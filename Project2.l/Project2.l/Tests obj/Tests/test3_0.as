        lw      3       4       c
        lw      2       3       Dog
        jalr    2       2
        add     4       0       1
Box     beq     0       0       Shim
Shim    noop
        add     0       3       0
        beq     1       5       Box
        nor     3       4       7
        nor     3       7       1
done    halt
        noop
c      .fill   -64
Dog    .fill   Liz
