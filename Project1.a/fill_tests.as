        lw 0 1 1
        lw 0 2 2
        lw 0 3 3
loop    add 1 2 4
        beq 3 4 loop
        sw 0 1 pig
        sw 0 1 3
pig .fill loop
