build:
	gcc -std=c11 script.c
	./a.out namdtoff

clean:
	rm -f ./a.out
