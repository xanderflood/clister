all: main.c
	gcc -O0 -ggdb3 main.c -I/usr/lib/llvm-4.0/include -lclang-4.0 -o clist

clean:
	rm -rf *.o

