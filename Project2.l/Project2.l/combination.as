func    lw          0        6        one       r6 = temp value, 1 | func to base = storing ret.add, n, r to stack
        sw          5        7        Stack     r5 = stack ptr, r7 = return, stack = store ret add
        add         6        5        5         stack ptr++
        sw          5        1        Stack     r1 = n, store n on stack
        add         6        5        5         stack ptr++
        sw          5        2        Stack     r2 = r, store r on stack
        add         6        5        5         stack ptr++
        beq         2        0        r1        r == 0 --> return 1 | base to recur = base case
        beq         2        1        r1        r == n --> return 1
        beq         2        6        rn        r == 1 --> return n | recur to next = set up recur (else statement in c)
        lw          0        6        neg1      r6 = -1 | else statement
        add         6        1        4         r4 = n - 1
        beq         2        4        rn        r == n - 1 --> return n
        add         1        6        1         n = n - 1
        lw          0        4        Caddr     r4 = start addy
        jalr        4        7                  first recur: (n-1, r)
        lw          0        6        one       r6 = 1
        sw          5        3        Stack     store return val on stack
        add         6        5        5         stack ptr++
        lw          0        6        neg1      r6 = -1
        add         2        6        2         r = r - 1
        lw          0        4        Caddr     load r4 with start addy
        jalr        4        7                  second recur: (n-1, r-1)
        lw          0        6        one       r6 = 1
        sw          5        3        Stack     store return val on stack
        add         6        5        5         stack ptr++
        lw          0        6        neg1      r6 = -1 | add two return vals together
        add         6        5        5         stack ptr--
        lw          5        4        Stack     r4 = ptr from stack: comb(n-1, r-1)
        add         6        5        5         stack ptr--
        lw          5        3        Stack     r3 = ptr from stack: comb(n-1, r)
        add         4        3        3         r3 = r4 + r3
        beq         0        0        exit      done with recur, return
rn      add         1        0        3         r3 = n | returns 1 | returns r
r1      lw          0        3        one       r3 = 1
exit    lw          0        6        neg1      r6 = -1
        add         6        5        5         stack ptr--
        lw          5        2        Stack     r2 = original r
        add         6        5        5         stack ptr--
        lw          5        1        Stack     r1 = original n
        add         6        5        5         stack ptr--
        lw          5        7        Stack     r7 = original return addy
        jalr        7        4                  return
Caddr   .fill       func
one     .fill       1
neg1    .fill       -1
