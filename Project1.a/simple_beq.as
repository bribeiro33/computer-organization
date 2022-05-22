        lw 0 1 zero
        lw 0 2 neg5
        lw 0 3 neg5
        lw 0 4 thou
        lw 0 5 ft
        lw 0 6 -12
        beq 2 3 cat
loop    nor 1 2 1
cat     nor 5 3 6
        beq 5 1 1
        beq 0 0 loop
        add 4 1 4
        noop
done    halt
ft .fill 15
thou .fill 160
neg5 .fill -5
zero .fill 0
