        lw 0 1 0
        lw 0 3 1
        lw 0 4 2
loop    beq 3 4 end
        noop
        beq 1 3 loop
end halt
