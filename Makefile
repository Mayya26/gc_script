build:
	gcc -std=c11 -lm -fopenmp script.c -o namd
	./namd namdtoff

clean:
	rm -f ./a.out *.xyz 



