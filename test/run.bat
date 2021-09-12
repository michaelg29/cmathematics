cd ../cmathematics
gcc -o ../test/a.exe ../test/main.c ^
    cmathematics.c ^
    data/encryption/aes.c ^
    data/hashing/sha1.c ^
    data/hashing/sha3.c ^
    graph/graph.c ^
    lib/avl.c ^
    lib/dynamicarray.c ^
    lib/functions.c ^
    lib/minheap.c ^
    lib/strstream.c ^
    linalg/matrix.c ^
    linalg/vec.c ^
    util/bigint.c ^
    util/exp_util.c ^
    util/expressions.c
cd ../test
a.exe