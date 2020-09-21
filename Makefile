all:
	gcc -Wall -o main *.c
.PHONY: badtest test ast 
badtest:
	./main --dump-tokens file.txt
test:
	./main --dump-tokens test.txt
ast:
	./main --dump-ast test.txt