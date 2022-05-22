        lw 0 1 num
        lw 0 7 zero
        lw 0 5 neg1
        lw 0 0 zero
        lw 0 2 2
        lw 0 3 4
        lw 0 4 1
loop    add 4 2 2
        add 1 0 0
        beq 2 3 end
        beq 7 7 loop
end     halt
zero    .fill 0
neg1    .fill -1
num     .fill 5
