         lw    0    1    data1
         lw    0    2    data2
         lw    0    3    data3
         noop
         noop
         noop
         beq    0    0    end
         add    1    2    4
         add    2    3    5
         nor    1    0    6
         nor    2    0    7
         noop
         noop
end      halt
data1    .fill    100
data2    .fill    200
data3    .fill    300
