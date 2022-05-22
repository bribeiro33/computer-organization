        lw 0 1 one
        lw 0 2 neg1
        beq 1 2 end
        add 1 1 1
        add 2 2 2
        noop
end     halt
one     .fill 1
neg1    .fill 2
