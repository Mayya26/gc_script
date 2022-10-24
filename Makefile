build:
	gcc -std=c11 -lm script.c -o namd
	./namd namdtoff

clean:
	rm -f ./a.out *.xyz 



