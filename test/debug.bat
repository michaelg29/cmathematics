cd ../cmathematics
gcc -g -o ../test/a.exe ../test/main.c vec.c matrix.c bigint.c
cd ../test
gdb a.exe