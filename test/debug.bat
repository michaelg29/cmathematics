cd ../cmathematics
gcc -g -o ../test/a.exe ../test/main.c cmathematics.c lib/avl.c lib/dynamicarray.c lib/functions.c lib/minheap.c lib/strstream.c vec.c matrix.c bigint.c exp_util.c expressions.c graph.c
cd ../test
gdb a.exe