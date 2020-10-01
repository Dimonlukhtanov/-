all:
	gcc -Wall -o main *.c
.PHONY: test ast asm

asm:
	./main --dump-asm BAD.txt
	nasm -felf64 main.asm -o main.o	
	gcc -no-pie main.o -o MAIN
	./MAIN
#badtest:
#	./main --dump-tokens BAD.txt
test:
	./main --dump-tokens BAD.txt
ast:
	./main --dump-ast BAD.txt