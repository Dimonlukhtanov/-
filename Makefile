all:
	gcc -Wall -o main *.c
.PHONY: test ast asm asmtest

asm:
	./main --dump-asm BAD.txt
	nasm -felf64 main.asm -o main.o	
	gcc -no-pie main.o -o MAIN
	./MAIN

asmtest:
	./main --dump-asm prog.txt
	nasm -felf64 main.asm -o main.o	
	gcc -no-pie main.o -o MAIN
	./MAIN
#badtest:
#	./main --dump-tokens BAD.txt
test:
	./main --dump-tokens BAD.txt
ast:
	./main --dump-ast BAD.txt