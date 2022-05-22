start   lw 0 1 data1
        lw 0 2 data2
        lw 0 3 data3
loop    add 1 2 1
        beq 1 3 done
        beq 0 0 loop
done halt
data1 .fill 0
data2 .fill 1
data3 .fill 5
