        lw      0   1   input
        lw      0   2   cnt
        lw      0   3   neg2
        lw      0   4   addr
        add     0   0   five
        jalr    4   6
        sw      0   5   100
        halt
func    add     5   1   5
        add     2   3   2
        beq     0   2   done
        beq     0   0   func
done    jalr    6   7
addr   .fill    func
input  .fill    3
cnt    .fill    10
neg2   .fill    -2
