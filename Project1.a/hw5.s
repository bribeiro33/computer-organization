        lw 0 1 pos1
        lw 0 2 neg1
        lw 0 3 size
        nor 1 1 4
loop    beq 0 3 done //beq0 4
        add 3 2 3
        lw 3 5 a
        nor 5 5 5
        nor 4 5 5
        beq 0 5 even //beq1 9
        add 6 1 6
        beq 0 0 else //beq2 11
even    add 7 1 7
else    beq 0 0 loop //beq3 13
done    halt
size    .fill 8
a       .fill 370
        .fill 108
        .fill 111
        .fill 110
        .fill 101
        .fill 108
        .fill 121
        .fill 109
pos1    .fill 1
neg1    .fill -1
