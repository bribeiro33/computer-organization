        lw  0   0   zero
        lw  0   1   24      //result
        lw  0   2   mcand   //mult1
        lw  0   3   mplier  //mult2
        lw  1   4   0       //mplier & index
        lw  0   5   24       //keep track to know when ends
        lw  5   6   1       //index
        noop
while   nor 3   0   3
        nor 6   0   6
        nor 3   6   4
        nor 3   0   3
        nor 6   0   6
        beq 4   0   mult
        add 1   2   1
        beq 0   0   mult
mult    add 4   5   5
        add 2   2   2
        add 6   6   6
        beq 3   5   end
        beq 0   0   while
end     halt
mcand   .fill  1103
mplier  .fill  7043
zero    .fill   0
one     .fill   1
