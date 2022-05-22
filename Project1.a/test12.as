        lw    0    1    data1
        lw    0    1    data2
        lw    1    3    data3
        lw    3    2    data1
        noop
        noop
        add    1    2    4
        add    2    3    5
        add    4    5    6
        halt
data1    .fill    10
data2    .fill    20
data3    .fill    30
