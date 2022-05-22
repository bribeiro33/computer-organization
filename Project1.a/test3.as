        lw 1 5 3
        lw 0 0 num
        jalr 0 5
start   add 0 5 0
        beq 0 0 start
        halt
num .fill 17
neg1 .fill -1
