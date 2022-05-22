        add 0 0 0
        add 7 7 7
        nor 0 0 0
        nor 7 7 7
        lw 0 0 0
        lw 7 7 65535
        sw 0 0 0
        sw 7 7 65535
        beq 0 0 -65535
        beq 7 7 65535
        jalr 0 0
        jalr 1000 7
        noop
        noop
        halt
min .fill -2147483648
max .fill 2147483647
