        lw    0    1    five
        lw    0    4    SubAdr
Stack   jalr    4    7
        beq    0    1    done
        beq    0    0    Stack
done    halt
five    .fill    5
