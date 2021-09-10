cd ../cmathematics
gcc -o ../test/a.exe ../test/main.c cmathematics.c lib/avl.c lib/dynamicarray.c lib/functions.c lib/minheap.c lib/strstream.c aes.c sha1.c sha3.c vec.c matrix.c bigint.c exp_util.c expressions.c graph.c
cd ../test
a.exe