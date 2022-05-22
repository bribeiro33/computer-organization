    lw  0  2  mcand
    lw  0  3  mplier
    lw  0  4  max
    lw  0  1  1
    lw  0   5   0
    lw  0   6   0
loop    beq  5  4  done
    nor  1  1  1
    nor  3  3  3
    nor  3  1  6
    beq  6  0  shift
    add  0  2  0
shift   lw   0  6  1
    nor  1  1  1
    nor  3  3  3
    add  2  2  2
    add  1  1  1
    add  5  6  5
    beq  5  5  loop
done    halt
mcand   .fill  32766
mplier  .fill  10383
max     .fill  15
