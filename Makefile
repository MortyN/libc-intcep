compile: example_compile
	gcc -Wall -fPIC -shared -o intercept.so intercept.c -ldl

example_compile:
	gcc exampleprogram/main.c -o exampleprogram/main.o

intercept_compile:
	gcc -Wall -fPIC -shared -o intercept.so intercept.c -ldl -ggdb -O0
