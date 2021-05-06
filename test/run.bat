cd ../cmathematics
gcc -o ../test/a.exe ../test/main.c cmathematics.c lib/avl.c lib/dynamicarray.c lib/strstream.c vec.c matrix.c bigint.c sy_util.c shuntingyard.c graph.c
cd ../test
a.exe