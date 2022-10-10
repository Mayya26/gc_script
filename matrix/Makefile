
all:	s21_matrix.a test

test:	s21_matrix.a
	gcc -std=c11 test.c s21_matrix.a -lm -lcheck -o test
	./test

s21_matrix.a: s21_matrix.o
	ar rcs s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a
	cp s21_matrix.a libs21_matrix.a

s21_matrix.o : s21_matrix.c s21_matrix.h
	gcc -std=c11 -c s21_matrix.c -os21_matrix.o

gcov_report:
	gcc -std=c11 --coverage -lm test.c s21_matrix.c -o test -lcheck
	./test
	lcov -t "test" -o test.info -c -d ./
	genhtml -o report test.info

clean:
	rm -rf test test *.a *.o *.out fizz *.gc* *.info report

check: s21_matrix.a
	cppcheck --enable=all --suppress=missingIncludeSystem --check-config *.h *.c
lint:	
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h
	#CK_FORK=no leaks --atExit -- ./test

valgrind: 
	gcc -lm -lcheck s21_matrix.c test.c -o val
	valgrind -s --trace-children=yes --leak-check=yes ./val

rebuild:
	make clean
	make test
