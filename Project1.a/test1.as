        lw 0 1 num
        lw 0 7 zero
        lw 0 5 neg1
        lw 0 0 zero
loop    add 1 5 3
        add 1 0 0
        beq 3 7 end
        beq 7 7 loop
end     halt
zero    .fill 0
neg1    .fill -1
num     .fill 5
