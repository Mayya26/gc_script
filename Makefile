build:
	gcc -std=c11 -lm matrix/s21_matrix.c script.c
	./a.out namdtoff

clean:
	rm -f ./a.out



