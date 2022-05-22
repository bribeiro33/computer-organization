            lw 0 1 fthou
            lw 0 2 neg3
            lw 0 3 six
            lw 0 4 zero
            lw 1 5 neg3
            lw 0 6 15
            lw 0 7 2
            lw 0 8 3
            beq 8 7 cat
dog         add 1 2 3
loop        add 5 2 3
            beq 5 1 loop
clown       add 1 1 1
            beq 1 2 clown
cookie      beq 7 7 cookie
            nor 1 1 1
            beq 1 5 dog
            sw 1 0 neg3
            sw 5 4 six
cat         sw 15 5 zero
            sw -13 2 6
            noop
            halt
neg3 .fill -3
fourt .fill 14
fthou .fill 5000
six .fill 6
zero .fill 0
