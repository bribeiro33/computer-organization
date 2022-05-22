        lw 0 1 fivet
        lw 0 2 neg3
        lw 0 3 six
        lw 0 4 zero
        lw 1 5 neg3
        lw 0 6 15
loop    add 5 2 3
        beq 5 1 loop
        add 1 1 1
        beq 1 2 3
        nor 4 3 5
        nor 1 1 1
        sw 1 0 neg3
        sw 5 4 six
        sw 15 5 zero
        sw -13 2 6
end     noop
halt
neg3 .fill -3
fourt .fill 14
fivet .fill 5000
six .fill 6
six .fill 6
zero .fill 0
